using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Windows;
using System.IO;

namespace Deng_shape_3D.Models
{
    public class Context
    {
        public string path { get; set; }
        public float pixelSize { get; set; }
        public float pixelSizeZ { get; set; }
        public float timeInterval { get; set; }
    }
    public static class ContextManager
    {
        private static Context context { get; set; }
        public static Context getContext()
        {
            return context;
        }

        public static void setPath(string path)
        {
            context = new Context
            {
                path = path
            };
        }

        public static void setMovieParameters(float pixelSize, float pixelSizeZ, float timeInterval)
        {
            context.pixelSize = pixelSize;
            context.pixelSizeZ = pixelSizeZ;
            context.timeInterval = timeInterval;
        }

        public static void writeToJson()
        {
            string json = JsonSerializer.Serialize(context);
            System.IO.File.WriteAllText(context.path + "/save.json", json);
        }

        public static bool loadFromJson(string file_path)
        {
            bool ret = true;
            string local_path = Path.GetDirectoryName(file_path);

            // Check that the data is still where it belongs, if not, report corruption:
            ret = Directory.Exists(local_path + "/.data_do_not_delete");

            if (ret)
            {
                string json = System.IO.File.ReadAllText(file_path);
                context = JsonSerializer.Deserialize<Context>(json);

                // If the path has changed, update it in the json object:
                if (local_path != context.path) context.path = local_path;
            }

            return ret;
        }

        public static void saveRawDataToLocalCache(string dataPath)
        {
            string destination = context.path + "/.data_do_not_delete/raw/";
            Directory.CreateDirectory(destination);

            Microsoft.VisualBasic.FileIO.FileSystem.CopyDirectory(dataPath, destination);
        }
    }
}
