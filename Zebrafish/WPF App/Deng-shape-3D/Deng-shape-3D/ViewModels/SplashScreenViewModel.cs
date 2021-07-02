using Deng_shape_3D.Models;

using System.IO;
using System.Windows.Forms;
using System.Windows.Input;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Windows.Documents;

namespace Deng_shape_3D.ViewModels
{
    class SplashScreenViewModel
    {
        private ICommand _openExistingCommand;
        public ICommand OpenExistingCommand
        {
            get
            {
                return _openExistingCommand ?? (_openExistingCommand = new CommandHandler(() => openExistingProject(), () => true));
            }
        }

        private ICommand _createNewCommand;
        public ICommand CreateNewCommand
        {
            get
            {
                return _createNewCommand ?? (_createNewCommand = new CommandHandler(() => createNewProject(), () => true));
            }
        }

        private void openExistingProject()
        {
            OpenFileDialog fdlg = new OpenFileDialog();
            fdlg.Title = "Select save.json File";
            fdlg.InitialDirectory = @"C:\";
            fdlg.Filter = "json file (*.json)|*.json";
            fdlg.FilterIndex = 2;
            fdlg.RestoreDirectory = true;
            if (fdlg.ShowDialog() == DialogResult.OK)
            {
                if (ContextManager.loadFromJson(fdlg.FileName))
                {
                    // Change view:
                    DashboardView dbv = new DashboardView();
                    App.Current.Windows[0].Close();
                    dbv.Show();
                }
                else
                {
                    MessageBox.Show("Error: Project has become corrupted. Check json file and ensure data was not deleted.");
                }
            }

        }

        private void createNewProject()
        {
            using (var fbd = new FolderBrowserDialog())
            {
                DialogResult result = fbd.ShowDialog();

                if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
                {
                    if (Directory.GetFiles(fbd.SelectedPath).Length == 0)
                    {
                        ContextManager.setPath(fbd.SelectedPath);
                        ContextManager.writeToJson();

                        // Once json successfully created, pull up the next view:
                        NewProjectView npv = new NewProjectView();
                        App.Current.Windows[0].Close();
                        npv.Show();
                    }
                    else
                    {
                        MessageBox.Show("Error: Directory is not empty!");
                    }
                }
            }
        }
    }
}
