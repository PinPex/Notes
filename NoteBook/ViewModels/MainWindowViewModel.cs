using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using NoteBook.Models;
using ReactiveUI;
using System.Reactive;
using System.Reactive.Linq;
using System.Linq;

namespace NoteBook.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        ViewModelBase page;

        public ViewModelBase Page
        {
            set => this.RaiseAndSetIfChanged(ref page, value);
            get => page;
        }

        public TaskListViewModel MainPage
        {
            get;
        }

        public MainWindowViewModel()
        {
            Page = MainPage = new TaskListViewModel();
            ObserveCommand = ReactiveCommand.Create<Note, int>((note) => openObservePage(note));
            DeleteCommand = ReactiveCommand.Create<Note, int>((note) => deleteNote(note));
        }
        public ReactiveCommand<Note, int> ObserveCommand { get; }
        public ReactiveCommand<Note, int> DeleteCommand { get; }

        public void openAddPage()
        {
            var taskPage = new TaskViewModel(new Note("", ""));
            Page = taskPage;
            Observable.Merge(taskPage.AddCommand, taskPage.CancelCommand).Take(1)
                .Subscribe((note) =>
                {
                    if(note.header != "")
                    {
                        bool added = MainPage.Notes.TryAdd(MainPage.CurrentDate, new List<Note> { note });
                        if (!added)
                        {
                            MainPage.Notes[MainPage.CurrentDate].Add(note);
                        }
                    }
                    MainPage.NoteList = new ObservableCollection<Note>(MainPage.Notes[MainPage.CurrentDate]);
                    Page = MainPage;
                });
        }

        public int openObservePage(Note selectedNote)
        {
            var taskPage = new TaskViewModel(selectedNote);
            Page = taskPage;
            Observable.Merge(taskPage.AddCommand, taskPage.CancelCommand).Take(1)
                .Subscribe((note) =>
                {
                    if (note.header != "")
                    {
                        selectedNote.Header = note.Header;
                        selectedNote.Description = note.Description;
                    }
                    MainPage.NoteList = new ObservableCollection<Note>(MainPage.Notes[MainPage.CurrentDate]);
                    Page = MainPage;
                });
            return 1;
        }

        public int deleteNote(Note selectedNote)
        {
            MainPage.Notes[MainPage.CurrentDate].Remove(selectedNote);
            MainPage.NoteList = new ObservableCollection<Note>(MainPage.Notes[MainPage.CurrentDate]);
            return 1;
        }
    }
}
