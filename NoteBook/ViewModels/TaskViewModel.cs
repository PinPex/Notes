using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ReactiveUI;
using System.Reactive;
using NoteBook.Models;

namespace NoteBook.ViewModels
{
    public class TaskViewModel : ViewModelBase
    {
        string header;
        string description;
        public TaskViewModel(Note note)
        {
            Header = note.header;
            Description = note.description;
            AddCommand = ReactiveCommand.Create(() => new Note(Header, Description));
            CancelCommand = ReactiveCommand.Create(() => new Note("", ""));
        }

        public ReactiveCommand<Unit, Note> AddCommand { get; }
        public ReactiveCommand<Unit, Note> CancelCommand { get; }
        public string Header 
        { 
            set => this.RaiseAndSetIfChanged(ref header, value);
            get => header;
        }

        public string Description
        {
            set => this.RaiseAndSetIfChanged(ref description, value);
            get => description;
        }
    }
}
