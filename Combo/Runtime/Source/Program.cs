using Combo;
using Runtime.Source;

class Program
{
    static void Main(string[] args)
    {
        Platform.Init();
        Events.AddBeginPlayEvent(() => {
            Logger.Info("LOD Test");
        });
        Environment.Exit(Core.Init());
    }
}