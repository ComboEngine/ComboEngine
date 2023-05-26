using System;
using System.Reflection;

namespace Sakura.Tools {
    public class ArgumentParser {
        public T accept<T>(string[] args) {
            Type typeParameterType = typeof(T);
            Object obj = Activator.CreateInstance(typeParameterType);
            foreach(FieldInfo field in typeParameterType.GetFields()) {
                int index = 0;
                foreach(string arg in args) {
                    if(arg.Equals("--" + field.Name)) {
                        try {
                            field.SetValue(obj,args[index+1]);
                        }catch(Exception e) {

                        }
                    }
                    index++;
                }
            }
            return (T)obj;
        }
    }
}