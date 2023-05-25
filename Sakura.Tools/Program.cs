using System;
using Astur.Tools;

namespace Astur.Tools {
    public class Program {
        public static void Main(string[] args) {
            ArgumentParser parser = new ArgumentParser();
            Options options = parser.accept<Options>(args);

            Console.WriteLine(options.action);
        }
    }
}