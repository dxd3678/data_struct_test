#nullable enable

using System.Text;
using StateSmith.Output.UserConfig;

namespace StateSmith.Output.Gil.Python;

public class GilToPython : IGilTranspiler
{
    private readonly StringBuilder fileSb = new();

    private readonly ICodeFileWriter codeFileWriter;
    private readonly RenderConfigPythonVars renderConfigPython;
    private readonly RenderConfigBaseVars renderConfig;
    private readonly IOutputInfo outputInfo;
    private readonly RoslynCompiler roslynCompiler;
    private readonly CodeStyleSettings codeStyleSettings;

    public GilToPython(IOutputInfo outputInfo, RenderConfigPythonVars renderConfigPython, RenderConfigBaseVars renderConfig, ICodeFileWriter codeFileWriter, RoslynCompiler roslynCompiler, CodeStyleSettings settings)
    {
        this.outputInfo = outputInfo;
        this.renderConfigPython = renderConfigPython;
        this.renderConfig = renderConfig;
        this.codeFileWriter = codeFileWriter;
        this.roslynCompiler = roslynCompiler;
        this.codeStyleSettings = settings;
    }

    public void TranspileAndOutputCode(string gilCode)
    {
        //File.WriteAllText($"{outputInfo.outputDirectory}{nameMangler.SmName}.gil.cs", programText);

        PythonGilVisitor gilVisitor = new(gilCode, fileSb, renderConfigPython, renderConfig, roslynCompiler, codeStyleSettings);
        gilVisitor.Process();

        PostProcessor.PostProcess(fileSb);

        codeFileWriter.WriteFile($"{outputInfo.OutputDirectory}{outputInfo.BaseFileName}.py", code: fileSb.ToString());
    }
}
