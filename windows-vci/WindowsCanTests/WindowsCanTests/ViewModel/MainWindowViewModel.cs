using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using WindowsCanTests.Model;

namespace WindowsCanTests.ViewModel
{
    class MissedMessageViewModel : ViewModelBase
    {
        private int missedCount;
        public int MissedCount
        {
            get { return missedCount; }
            set { missedCount = value; NotifyPropertyChanged(nameof(MissedCount)); }
        }

        private int receivedCount;
        public int ReceivedCount
        {
            get { return receivedCount; }
            set { receivedCount = value; NotifyPropertyChanged(nameof(ReceivedCount)); }
        }
    }

    class MainWindowViewModel : ViewModelBase
    {
        private bool _connected;
        public bool Connected
        {
            get { return _connected; }
            set { _connected = value; NotifyPropertyChanged(nameof(Connected)); }
        }

        private bool _testsAreRunning;
        public bool TestsAreRunning 
        {
            get { return _testsAreRunning; }
            private set { _testsAreRunning = value; NotifyPropertyChanged(nameof(TestsAreRunning)); }
        }

        public bool AutoIncrement { get; set; }
        public int IncrementMessagesCount { get; set; }

        private int _delay;
        public int Delay
        {
            get { return _delay; }
            set 
            {
                _delay = value;
                NotifyPropertyChanged(nameof(Delay));
                TestRunner.UsedDelay = Delay; 
                SentMessages = 0;
            }
        }

        private int _sentMessages;
        public int SentMessages
        {
            get { return _sentMessages; }
            set { _sentMessages = value; NotifyPropertyChanged(nameof(SentMessages)); }
        }

        private List<int> _storedDelays = new List<int>();
        public ObservableDictionary<int, MissedMessageViewModel> SkippedCountByDelay { get; } = new ObservableDictionary<int, MissedMessageViewModel>();

        public ICommand StartTestsCommand => new RelayCommand(async (_) => await StartTests());
        public ICommand StopTestsCommand => new RelayCommand((_) => StopTests());


        private TestRunner TestRunner { get; set; } = new TestRunner();

        public MainWindowViewModel()
        {
            AutoIncrement = true;
            IncrementMessagesCount = 1000;
            Delay = 95;

            Connected = CanManager.Instance.Connected;
            CanManager.Instance.ConnectedStatusChanged += () =>
            {
                Connected = CanManager.Instance.Connected;
            };

            TestRunner.SentMessage += SentMessage;
            TestRunner.SkippedMessage += SkippedMessage;
        }

        private void SkippedMessage(int obj)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                if (!SkippedCountByDelay.ContainsKey(obj))
                    SkippedCountByDelay.Add(obj, new MissedMessageViewModel());
                SkippedCountByDelay[obj].MissedCount++;
            });
        }

        private void SentMessage(int obj)
        {
            SentMessages++;
            Application.Current.Dispatcher.Invoke(() =>
            {
                if (!SkippedCountByDelay.ContainsKey(obj))
                    SkippedCountByDelay.Add(obj, new MissedMessageViewModel());
                SkippedCountByDelay[obj].ReceivedCount++;
                if (AutoIncrement && (SentMessages % IncrementMessagesCount == 0))
                {
                    Delay += 1;
                }
            });
        }

        private async Task StartTests()
        {
            SentMessages = 0;
            TestsAreRunning = true;
            try
            {
                await TestRunner.SendMessagesAsync();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
            TestsAreRunning = false;
        }

        private void StopTests()
        {
            TestRunner.StopTests();
        }
    }
}
