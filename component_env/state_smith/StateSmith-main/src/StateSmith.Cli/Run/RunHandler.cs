using Spectre.Console;
using StateSmith.Cli.Manifest;
using StateSmith.Common;
using StateSmith.Runner;
using System;
using System.IO;

namespace StateSmith.Cli.Run;

public class RunHandler
{
    public SsCsxDiagramFileFinder Finder;

    private string _manifestDirectory;
    private string _SearchDirectory => _manifestDirectory;

    private RunInfoStore _runInfoStore;
    private RunInfoDataBase _runInfoDataBase;
    private IAnsiConsole _console;
    private readonly DiagramOptions _diagramOptions;
    private RunConsole _runConsole;
    private RunHandlerOptions _runHandlerOptions;
    private CsxRunner _csxRunner;
    private DiagramRunner _diagramRunner;

    public RunHandler(IAnsiConsole console, string dirOrManifestPath, DiagramOptions diagramOptions, RunHandlerOptions runHandlerOptions)
    {
        Finder = new SsCsxDiagramFileFinder();

        _console = console;
        _runHandlerOptions = runHandlerOptions;
        this._diagramOptions = diagramOptions;
        dirOrManifestPath = Path.GetFullPath(dirOrManifestPath);

        FileAttributes attr = File.GetAttributes(dirOrManifestPath);
        if (attr.HasFlag(FileAttributes.Directory))
            dirOrManifestPath = PathUtils.EnsureDirEndingSeperator(dirOrManifestPath);

        _manifestDirectory = Path.GetDirectoryName(dirOrManifestPath).ThrowIfNull();
        _runInfoStore = new RunInfoStore(dirOrManifestPath);
        _runInfoDataBase = new RunInfoDataBase(dirOrManifestPath, console);
        _runConsole = new RunConsole(_console);
        _csxRunner = new CsxRunner(_runConsole, _SearchDirectory, _runHandlerOptions);
        _diagramRunner = new DiagramRunner(_runConsole, _diagramOptions, searchDirectory: _SearchDirectory, _runHandlerOptions);
    }

    private bool IsVerbose => _runHandlerOptions.Verbose;
    private bool IsNoCsx => _runHandlerOptions.NoCsx;
    private bool IsRebuild => _runHandlerOptions.Rebuild;

    public void AddFromManifest(ManifestData manifest)
    {
        Finder.AddIncludePatterns(manifest.RunManifest.IncludePathGlobs);
        Finder.AddExcludePatterns(manifest.RunManifest.ExcludePathGlobs);
    }

    public void Run()
    {
        try
        {
            ReadPastRunInfoDatabase();

            if (_runHandlerOptions.Watch)
            {
                Watch();
            }
            else
            {
                ExecuteNormalRun();
            }
        }
        catch (Exception ex)
        {
            if (ex is not FinishedWithFailureException)
            {
                _console.WriteException(ex);
            }

            Environment.ExitCode = 1;   // TODO - fix. this is not ideal. Might mess up unit tests.
        }
    }

    private void ReadPastRunInfoDatabase()
    {
        RunInfoStore? readRunInfo = _runInfoDataBase.ReadRunInfoDatabase();

        if (readRunInfo != null)
        {
            _runInfoStore = readRunInfo.DeepCopy();
        }
    }

    private void Watch()
    {
        SsCsxDiagramFileFinder.ScanResults scanResults = ScanForFiles();
        PrintInterstingScanInfo(scanResults);

        var watchRunner = new WatchRunner(_runConsole, _csxRunner, _diagramRunner, _runInfoStore, _runHandlerOptions, _runInfoDataBase, searchDirectory: _SearchDirectory);
        watchRunner.Run(scanResults);
    }

    private void ExecuteNormalRun()
    {
        SsCsxDiagramFileFinder.ScanResults scanResults = ScanForFiles();

        _csxRunner.RunScriptsIfNeeded(scanResults.targetCsxFiles, _runInfoStore, out bool ranFiles);
        ranFiles |= _diagramRunner.Run(scanResults.targetDiagramFiles, _runInfoStore);

        if (ranFiles)
        {
            _runInfoDataBase.PersistRunInfo(_runInfoStore, printMessage: true);
        }

        PrintInterstingScanInfo(scanResults);
    }

    private SsCsxDiagramFileFinder.ScanResults ScanForFiles()
    {
        return Finder.Scan(searchDirectory: _SearchDirectory);
    }

    private void PrintInterstingScanInfo(SsCsxDiagramFileFinder.ScanResults scanResults)
    {
        bool spacerPrinted = false;

        void PrintSpacerIfNeeded()
        {
            if (!spacerPrinted)
            {
                _runConsole.MarkupLine("");
                spacerPrinted = true;
            }
        }

        // print ignored files
        if (IsVerbose && scanResults.ignoredFiles.Count > 0)
        {
            PrintSpacerIfNeeded();
            _runConsole.QuietMarkupLine("Ignored files: " + string.Join(", ", scanResults.ignoredFiles));
        }

        // print non-matching files
        if (IsVerbose && scanResults.nonMatchingFiles.Count > 0)
        {
            PrintSpacerIfNeeded();
            _runConsole.QuietMarkupLine("Non-matching files: " + string.Join(", ", scanResults.nonMatchingFiles));
        }

        // print broken svg files always (ignore verbose)
        if (scanResults.brokenDrawioSvgFiles.Count > 0)
        {
            _runConsole.WriteLine(""); // always add a spacer
            _runConsole.WarnMarkupLine("!!! Broken drawio.svg files found !!!");

            foreach (var item in scanResults.brokenDrawioSvgFiles)
            {
                _runConsole.MarkupLine($"  - {item}");
            }

            _runConsole.MarkupLine("  - see [blue][u]https://github.com/StateSmith/StateSmith/issues/341[/][/]");
        }
    }
}
