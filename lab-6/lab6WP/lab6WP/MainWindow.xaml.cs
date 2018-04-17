using System;
using System.Xml;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace lab6WP
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            AddTaskDialog dlg = new AddTaskDialog();
            if (dlg.ShowDialog() == true)
            {
                // Get document
                XmlDocument document = ((XmlDataProvider)FindResource("tasks")).Document;
                // Create Task element
                XmlElement task = document.CreateElement("Task");
                // Add properties of the new task
                XmlElement name = document.CreateElement("Name");
                name.InnerText = dlg.TaskTitle.Text;
                task.AppendChild(name);
                XmlElement priority = document.CreateElement("Priority");
                priority.InnerText = dlg.TaskPriority.Text;
                task.AppendChild(priority);
                XmlElement done = document.CreateElement("Done");
                done.InnerText = "No";
                task.AppendChild(done);
                XmlElement description = document.CreateElement("Description");
                description.InnerText = dlg.TaskDescription.Text;
                task.AppendChild(description);
                // Append the new task to document
                document.DocumentElement.AppendChild(task);
            }
        }

        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (TaskListBox.SelectedIndex != -1)
            {
                XmlElement task = (XmlElement)TaskListBox.SelectedItem;
                task.ParentNode.RemoveChild(task);
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            XmlDataProvider tasks = (XmlDataProvider)FindResource("tasks");
            tasks.Document.Save("Tasks.xml");
        }
    }
}
