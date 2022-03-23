using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using NoteBook.ViewModels;
using System;
using Avalonia.Interactivity;

namespace NoteBook.Views
{
    public partial class TaskListView : UserControl
    {
        public TaskListView()
        {
            InitializeComponent();
            this.FindControl<DatePicker>("DateP").SelectedDateChanged += delegate
            {
                DateTimeOffset? a = this.FindControl<DatePicker>("DateP").SelectedDate;
                var context = this.DataContext as TaskListViewModel;
                if (context != null)
                    context.CurrentDate = a;
            };
            //this.FindControl<DatePicker>("DateP").SelectedDate = DateTime.Today;
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }
    }
}
