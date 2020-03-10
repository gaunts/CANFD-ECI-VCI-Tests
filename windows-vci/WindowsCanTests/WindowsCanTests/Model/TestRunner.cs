using Ixxat.Vci4;
using Ixxat.Vci4.Bal.Can;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace WindowsCanTests.Model
{
    class TestRunner
    {
        private Timer _timeoutTimer = new Timer();
        private bool _timedoutFlag = false;

        private bool _stopTestsFlag = false;
        private bool _waitingForPong = false;

        public event Action<int> SkippedMessage;
        public event Action<int> SentMessage;
        public int UsedDelay { get; set; }

        private enum MessageTypes
        {
            Ping = 0x01,
            Pong = 0x02,
            Test = 0x03,
            Skip = 0x04
        }

        public TestRunner()
        {
            CanManager.Instance.ReceivedMessage += ReceivedMessage;
            _timeoutTimer.Interval = TimeSpan.FromSeconds(3).TotalMilliseconds;
            _timeoutTimer.AutoReset = false;
            _timeoutTimer.Elapsed += ((_, __) => _timedoutFlag = true);
        }

        private void ReceivedMessage(ICanMessage2 message)
        {
            if ((MessageTypes)message[0] == MessageTypes.Pong)
            {
                _waitingForPong = false;
                _timeoutTimer?.Stop();
            }
            else if ((MessageTypes)message[0] == MessageTypes.Skip)
            {
                SkippedMessage?.Invoke(UsedDelay);
            }
        }

        private ICanMessage2 GetBaseMessage()
        {
            IMessageFactory factory = VciServer.Instance().MsgFactory;
            ICanMessage2 message = (ICanMessage2)factory.CreateMsg(typeof(ICanMessage2));

            message.TimeStamp = 0;
            message.Identifier = 2;
            message.FrameType = CanMsgFrameType.Data;
            message.SelfReceptionRequest = false;
            message.ExtendedFrameFormat = false;
            message.DataLength = 2;
            return message;
        }

        private ICanMessage2 GetPingMessage()
        {   
            ICanMessage2 msg = GetBaseMessage();
            msg[0] = (byte)MessageTypes.Ping;
            msg[1] = 0x00;
            return msg;
        }

        private ICanMessage2 GetTestMessage()
        {
            ICanMessage2 msg = GetBaseMessage();
            msg[0] = (byte)MessageTypes.Test;
            msg[1] = 0x00;
            return msg;
        }

        public async Task SendMessagesAsync()
        {
            _stopTestsFlag = false;

            await Task.Run(async () =>
            {
                if (!CheckCommunication())
                {
                    throw new Exception("Could not connect to the tested machine");
                }

                ICanMessage2 testMessage = GetTestMessage();
                uint sentMessages = 0;
                while (!_stopTestsFlag)
                {
                    testMessage[1] = (byte)(sentMessages % 0xFF);
                    CanManager.Instance.SendMessage(testMessage);
                    SentMessage?.Invoke(UsedDelay);
                    sentMessages++;
                    await Task.Delay(UsedDelay);
                }
            });
        }

        private bool CheckCommunication()
        {
            _waitingForPong = true;
            _timedoutFlag = false;
            _timeoutTimer.Start();
            CanManager.Instance.SendMessage(GetPingMessage());

            while (_timedoutFlag == false && _waitingForPong);

            if (_timedoutFlag == true)
                return false;
            return true;
        }

        public void StopTests()
        {
            _stopTestsFlag = true;
        }
    }
}
