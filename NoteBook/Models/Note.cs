using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NoteBook.Models
{
    public class Note
    {
        public string header;
        public string description;

        public Note(string header_, string description_)
        {
            header = header_;
            description = description_;
        }

        public string Header 
        { 
            get => header; 
            set => header = value; 
        }

        public string Description 
        {
            get => description; 
            set => description = value; 
        }
    }
}
