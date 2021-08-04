using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common
{
    public enum PlayerState : ushort
    {
        None = 0,
        NotReady,
        Setting,
        Ready,
        Loading,
        PlayWaiting,
        Playing,
        GameFinish,
        LeaveGame,
    };
}
