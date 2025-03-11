using System.Text;

namespace StateSmith.Output;

public class CodeStyleSettings
{
    public virtual string Indent1 => "    ";

    public virtual string Newline => "\n";

    /// <summary>
    /// NOTE! Changing this to false will break python transpiler.
    /// </summary>
    public virtual bool BracesOnNewLines => true;

    public virtual void Indent(StringBuilder sb, int count = 1)
    {
        for (int i = 0; i < count; i++)
        {
            sb.Append(Indent1);
        }
    }
}
