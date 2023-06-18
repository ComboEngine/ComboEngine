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

        Events.AddUpdateEvent(() =>
        {
            Interop.ExposeComponents();
        });
        Environment.Exit(Core.Init());
    }
}