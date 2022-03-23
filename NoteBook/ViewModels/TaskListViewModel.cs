using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using NoteBook.Models;
using ReactiveUI;
using System.Reactive;

namespace NoteBook.ViewModels
{
    public class TaskListViewModel : ViewModelBase
    {
        DateTimeOffset? currentDate;
        public TaskListViewModel() // DateTimeOffset?
        {
            CurrentDate = DateTime.Today;
            NoteList = new ObservableCollection<Note>(notes[currentDate]);   
        }
        public DateTimeOffset? CurrentDate
        {
            set
            {
                this.RaiseAndSetIfChanged(ref currentDate, value);
                bool added = notes.TryAdd(CurrentDate, new List<Note> {});
                NoteList = new ObservableCollection<Note>(notes[currentDate]);
            }
            get => currentDate;
        }

        ObservableCollection<Note> noteList;

        Dictionary<DateTimeOffset?, List<Note>> notes = new Dictionary<DateTimeOffset?, List<Note>>() { { DateTime.Today, new List<Note>() } };

        public ObservableCollection<Note> NoteList
        {
            get => noteList;
            set
            {
                this.RaiseAndSetIfChanged(ref noteList, value);
            }
        }

        public Dictionary<DateTimeOffset?, List<Note>> Notes
        {
            get => notes;
        }
    }
}
