using Ixxat.Vci4;
using Ixxat.Vci4.Bal;
using Ixxat.Vci4.Bal.Can;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Windows;

namespace WindowsCanTests
{
    public class CanManager
    {
        public event Action<ICanMessage2> ReceivedMessage;
        public event Action ConnectedStatusChanged;
        public bool Connected { get; private set; }

        private IBalObject _balObject;
        private ICanChannel2 _channel;
        private ICanMessageWriter _writer;
        private ICanMessageReader _reader;
        private ICanControl2 _control;
        private IVciDeviceList _deviceList;

        private Thread _readerThread;
        private Thread _callingThread;

        private AutoResetEvent _connectionEvent;

        private static CanManager _instance;
        public static CanManager Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new CanManager();
                return _instance;
            }
        }

        private CanManager()
        {
            _connectionEvent = new AutoResetEvent(true);

            using (IVciDeviceManager deviceManager = VciServer.Instance().DeviceManager)
            {
                _deviceList = deviceManager.GetDeviceList();
                _deviceList.AssignEvent(_connectionEvent);
            }

            Thread t = new Thread(ConnectionThread);
            t.IsBackground = true;
            t.Start();
        }

        private void Dispose()
        {
            _balObject?.Dispose();
            _channel?.Dispose();
            _writer?.Dispose();
            _reader?.Dispose();
            _control?.Dispose();
            _readerThread?.Abort();

            _balObject = null;
            _channel = null;
            _writer = null;
            _reader = null;
            _control = null;
            _readerThread = null;
        }

        private bool Connect()
        {
            Dispose();
            _callingThread = Thread.CurrentThread;

            this.Connected = false;
            ConnectedStatusChanged?.Invoke();

            _balObject = GetFirstAvailableCanObject();

            if (_balObject == null)
                return false;

            Setup();
            StartChannels();

            this.Connected = true;
            ConnectedStatusChanged?.Invoke();

            return true;
        }

        private IBalObject GetFirstAvailableCanObject()
        {
            IBalObject balObject = null;

            foreach (IVciDevice device in _deviceList)
            {
                try
                {
                    balObject = device.OpenBusAccessLayer();
                    device.Dispose();
                    break;
                }
                catch {}
            }
            if (balObject == null)
                Debug.WriteLine("Could not get a Bal object");

            return balObject;
        }

        private void StartChannels()
        {
            _channel = (ICanChannel2)_balObject.OpenSocket(0, typeof(ICanChannel2));
            _channel.Initialize(3, 3, 0, CanFilterModes.Pass, false);
            _channel.Activate();
            _writer = _channel.GetMessageWriter();
            _reader = _channel.GetMessageReader();

            _readerThread = new Thread(ReadThread);
            _readerThread.IsBackground = true;
            _readerThread.Start();
        }

        private void Setup()
        {
            _control = (ICanControl2)_balObject.OpenSocket(0, typeof(ICanControl2));
            var bitRate = new CanBitrate2(CanBitrateMode.Raw, 2, 63, 16, 16, 0);
            var extendedBitRate = new CanBitrate2(CanBitrateMode.Raw, 2, 7, 2, 2, 0);

            _control.InitLine(CanOperatingModes.Standard | CanOperatingModes.ErrFrame,
                CanExtendedOperatingModes.FastDataRate, CanFilterModes.Pass, 0, CanFilterModes.Pass, 0, bitRate, extendedBitRate);
            _control.StartLine();
        }

        private void ConnectionThread()
        {
            _connectionEvent.WaitOne();
            Connect();
            ConnectionThread();
        }

        private void ReadMessages()
        {
            if (_reader.FillCount > 0)
            {
                while (_reader.ReadMessage(out ICanMessage2 message))
                {
                    if (message.Identifier != 1)
                        continue;
                    Debug.WriteLine($"Received {message.ToString()}");
                    ReceivedMessage?.Invoke(message);
                }
            }
        }

        private void ReadThread()
        {
            try
            {
                while (true)
                {
                    ReadMessages();
                    Thread.Sleep(1);
                }
            }
            catch
            {
            }
        }

        public bool SendMessage(ICanMessage2 message)
        {
            if (message.Identifier == 0)
                return false;
            if (_writer == null)
                return false;
            return _writer.SendMessage(message);
        }
    }
}
