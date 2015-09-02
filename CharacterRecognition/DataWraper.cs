using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using CharacterRecognition.Annotations;

namespace CharacterRecognition
{
    class DataWraper : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private string _signetType;
        private string _signetSize;
        private string _protectionType;
        private string _similarityDegree;
        private string _filename;
        private string _filesize;
        private string _filetype;

        public DataWraper()
        {
            _signetType = "";
            _signetSize = "";
            _protectionType = "";
            _similarityDegree = "";
            _filename = "";
            _filesize = "";
            _filetype = "";
        }
        public string SignetType
        {
            get
            {
                return _signetType;
            }
            set
            {
                if (_signetType != value)
                {
                    _signetType = value;
                    OnPropertyChanged("SignetType");
                }
            }
        }

        public string SignetSize
        {
            get
            {
                return _signetSize;
            }
            set
            {
                if (_signetSize != value)
                {
                    _signetSize = value;
                    OnPropertyChanged("SignetSize");
                }
            }
        }

        public string ProtectionType
        {
            get
            {
                return _protectionType;
            }
            set
            {
                if (_protectionType != value)
                {
                    _protectionType = value;
                    OnPropertyChanged("ProtectionType");
                }
            }
        }

        public string SimilarityDegree
        {
            get
            {
                return _similarityDegree;
            }
            set
            {
                if (_similarityDegree != value)
                {
                    _similarityDegree = value;
                    OnPropertyChanged("SimilarityDegree");
                }
            }
        }

        public string Filename
        {
            get
            {
                return _filename;
            }
            set
            {
                if (_filename != value)
                {
                    _filename = value;
                    OnPropertyChanged("Filename");
                }
            }
        }

        public string Filesize
        {
            get
            {
                return _filesize;
            }
            set
            {
                if (_filesize != value)
                {
                    _filesize = value;
                    OnPropertyChanged("Filesize");
                }
            }
        }

        public string Filetype
        {
            get
            {
                return _filetype;
            }
            set
            {
                if (_filetype != value)
                {
                    _filetype = value;
                    OnPropertyChanged("Filetype");
                }
            }
        }

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
