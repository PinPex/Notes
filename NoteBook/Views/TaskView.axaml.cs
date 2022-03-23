using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using NoteBook.ViewModels;

namespace NoteBook.Views
{
    public partial class TaskView : UserControl
    {
        public TaskView()
        {
            InitializeComponent();

            this.FindControl<Button>("Ok").Click += delegate
            {
                var context = this.DataContext as TaskViewModel;
                if (this.FindControl<TextBox>("Header").Text != "")
                {
                    context.Header = this.FindControl<TextBox>("Header").Text;
                    context.Description = this.FindControl<TextBox>("Description").Text;
                }
            };
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }
    }
}
