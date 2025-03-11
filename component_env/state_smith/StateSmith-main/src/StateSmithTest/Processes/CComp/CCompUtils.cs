#nullable enable

using System.IO;

namespace StateSmithTest.Processes.CComp;

public class CCompUtils
{
    public static string GetCompilerPathOrDefault(CCompRequest request, string commandDefault)
    {
        // note: user could have set the env var to an empty string to disable it
        if (string.IsNullOrWhiteSpace(request.CompilerPath))
        {
            return commandDefault;
        }
        else
        {
            return request.CompilerPath;
        }
    }

    public static void AddGccClangArgs(SimpleProcess process, CCompRequest request)
    {
        process.Args += "-Wall ";

        if (request.IsCpp)
        {
            process.Args += "-std=c++11 ";
        }
        else
        {
            process.Args += "-std=c11 "; // only needed for test static asserts
        }

        foreach (var flag in request.Flags)
        {
            switch (flag)
            {
                case CCompRequest.FlagId.IgnoreUnusedFunctions:
                    process.Args += "-Wno-unused-function ";
                    break;

                default:
                    throw new System.Exception($"Unknown flag: {flag}");
            }
        }
    }

    /// <summary>
    /// Runs a default Unix executable named "a.out" in the working directory.
    /// </summary>
    /// <param name="runArgs"></param>
    /// <param name="workingDirectory"></param>
    /// <returns></returns>
    public static SimpleProcess RunDefaultUnixExecutable(string runArgs, string workingDirectory)
    {
        SimpleProcess process = SetupRunDefaultUnixExecutable(runArgs, workingDirectory);
        process.Run(timeoutMs: SimpleProcess.DefaultLongTimeoutMs);
        return process;
    }

    public static SimpleProcess SetupRunDefaultUnixExecutable(string runArgs, string workingDirectory)
    {
        return new()
        {
            ProgramPath = Path.Combine(workingDirectory, "a.out"),
            WorkingDirectory = workingDirectory,
            Args = runArgs
        };
    }

    public static SimpleProcess SetupDefaultProcess(CCompRequest request, string command)
    {
        SimpleProcess process = new()
        {
            ProgramPath = GetCompilerPathOrDefault(request, command),
            WorkingDirectory = request.WorkingDirectory
        };

        return process;
    }

    public static void AddSourceFilesArgs(SimpleProcess process, CCompRequest request)
    {
        process.Args += string.Join(" ", request.SourceFiles);
    }

    public static void AddIncludePathsArgs(SimpleProcess process, CCompRequest request)
    {
        foreach (var includePath in request.IncludePaths)
        {
            process.Args += $"-I {includePath} ";
        }
    }

    public static void GccClangCompile(CCompRequest request, CCompilerId cCompilerId)
    {
        SimpleProcess process = GccClangSetup(request, cCompilerId);
        process.Run(timeoutMs: SimpleProcess.DefaultLongTimeoutMs);
    }

    public static SimpleProcess GccClangSetup(CCompRequest request, CCompilerId cCompilerId)
    {
        string command;

        if (cCompilerId == CCompilerId.GCC)
        {
            command = request.IsCpp ? "g++" : "gcc";
        }
        else if (cCompilerId == CCompilerId.CLANG)
        {
            command = request.IsCpp ? "clang++" : "clang";
        }
        else
        {
            throw new System.Exception($"Unknown compiler id: {cCompilerId}");
        }

        SimpleProcess process = SetupDefaultProcess(request, command: command);
        AddGccClangArgs(process, request);
        AddIncludePathsArgs(process, request);
        AddSourceFilesArgs(process, request);
        return process;
    }
}
