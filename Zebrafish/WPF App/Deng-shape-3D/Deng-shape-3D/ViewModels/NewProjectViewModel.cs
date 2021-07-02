using Deng_shape_3D.Models;

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Input;

namespace Deng_shape_3D.ViewModels
{
    public class NewProjectViewModel : INotifyPropertyChanged
    {
        //INotifyPropertyChanged implementation:
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(String propertyName)
        {
            PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        //Question button ICommand:
        private ICommand _questionMarkPressed;
        public ICommand QuestionMarkPressed
        {
            get
            {
                return _questionMarkPressed ?? (_questionMarkPressed = new CommandHandler(() => displayQuestionMarkDialog(), () => true));
            }
        }

        //Continue button ICommand:
        private ICommand _continuePressed;
        public ICommand ContinuePressed
        {
            get
            {
                return _continuePressed ?? (_continuePressed = new CommandHandler(() => goToNextScreen(), () => true));
            }
        }

        //Continue button ICommand:
        private ICommand _backPressed;
        public ICommand BackPressed
        {
            get
            {
                return _backPressed ?? (_backPressed = new CommandHandler(() => goToSplashScreen(), () => true));
            }
        }

        //Browse button ICommand:
        private ICommand _browsePressed;
        public ICommand BrowsePressed
        {
            get
            {
                return _browsePressed ?? (_browsePressed = new CommandHandler(() => browseForPath(), () => true));
            }
        }

        public string ChannelPath
        {
            get; set;
        }

        public string PixelSize
        {
            get; set;
        }

        public string PixelSizeZ
        {
            get; set;
        }

        public string TimeInterval
        {
            get; set;
        }

        private void browseForPath()
        {
            using (var fbd = new FolderBrowserDialog())
            {
                DialogResult result = fbd.ShowDialog();

                if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
                {
                    ChannelPath = fbd.SelectedPath;
                    OnPropertyChanged("ChannelPath");

                    foreach (string directory in Directory.GetDirectories(fbd.SelectedPath))
                    {
                        //MessageBox.Show(directory);
                    }
                }
            }
        }

        private void goToNextScreen()
        {
            // First check if the given path equals null:
            bool pathIsValid = ChannelPath != null;

            // If the path is not null, check if directory is properly laid out:
            if (pathIsValid) pathIsValid = isListInIntegerOrder(Directory.GetDirectories(ChannelPath).ToList());

            // If directory is properly laid out, check that all subfolders are correctly laid out:
            if (pathIsValid)
            {
                foreach(string subdirectory in Directory.GetDirectories(ChannelPath))
                {
                    pathIsValid = pathIsValid && isListInIntegerOrder(Directory.GetFiles(subdirectory).ToList());
                }
            }

            if (pathIsValid)
            {
                bool bParseFailed = false;
                float fpixelSize = 0, fpixelSizeZ = 0, ftimeInterval = 0;
                if (PixelSize != null && PixelSizeZ != null && TimeInterval != null)
                {
                    // Try to parse each of the movie parameters:
                    try { fpixelSize = float.Parse(PixelSize); }
                    catch (FormatException) { bParseFailed = true; }

                    try { fpixelSizeZ = float.Parse(PixelSizeZ); }
                    catch (FormatException) { bParseFailed = true; }

                    try { ftimeInterval = float.Parse(TimeInterval); }
                    catch (FormatException) { bParseFailed = true; }
                }
                else
                {
                    bParseFailed = true;
                }

                // If parse successful, then switch screen, otherwise fail:
                if (!bParseFailed)
                {
                    // Save data so that if it moves it is not a problem:
                    ContextManager.saveRawDataToLocalCache(ChannelPath);

                    // Change view:
                    DashboardView dbv = new DashboardView();
                    App.Current.Windows[0].Close();
                    dbv.Show();
                }
                else
                {
                    MessageBox.Show("Failed to parse Movie Information: please enter reasonable values.");
                }
            }
            else
            {
                MessageBox.Show("Error: A valid directory must be provided. Click ? button for details.");
            }
        }

        private void displayQuestionMarkDialog()
        {
            MessageBox.Show("Select a folder containing subfolders 1, 2, .. N where each index represents the time. Each subfolder should contain tif files named 1.tif, 2.tif, .. n.tif where each index represents the z-layer height.");
        }

        private bool isListInIntegerOrder(List<string> lst)
        {
            // Ensure that there is at least one file or subdirectory:
            bool ret = lst.Count() > 0;

            List<int> i_names = new List<int>();

            if (ret)
            {
                for (int i = 0; i < lst.Count(); i++)
                {
                    string[] tmp = lst[i].Split('\\');
                    // In case we are checking the tif files (and not the directories):
                    string name = tmp[tmp.Count() - 1].Replace(".tif", "");
                    int j;
                    try
                    {
                        j = Int32.Parse(name);
                    }
                    catch (FormatException)
                    {
                        ret = false;
                        break;
                    }
                    i_names.Add(j);
                }
            }

            //Assuming all names were valid integers, check to see they are consecutive and start from 1:
            if (ret)
            {
                i_names.Sort();
                bool isConsecutive = !i_names.Select((i, j) => i - j).Distinct().Skip(1).Any();
                var ugh = i_names.Select(i => i < 1);
                bool greaterThanZero = !i_names.Select(i => i < 1).Contains(true);
                ret = isConsecutive && greaterThanZero;
            }

            return ret;
        }

        private void goToSplashScreen()
        {
            //Delete the json file, as it is no longer useful:
            File.Delete(ContextManager.getContext().path + "/save.json");

            //Go bck to the splash screen:
            SplashScreen ss = new SplashScreen();
            App.Current.Windows[0].Close();
            ss.Show();
        }
    }
}
