using Combo;
using Runtime.Source;

class Program
{
    static void Main(string[] args)
    {
        Platform.Init();
        Events.AddBeginPlayEvent(() => {
            Logger.Info("LOD Test");

            Events.AddUpdateEvent(() =>
            {
                Interop.ExposeComponents();
                //string[] array = Bindings.Interop_GetUpdateComponentsList();
                //foreach (string a in array)
                //{
                    //Console.WriteLine(a);
                //}
            });
        });
        Environment.Exit(Core.Init());
    }
}