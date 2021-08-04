using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using rumda.Protocol;

namespace Common
{
    class PlayerInfo
    {
        public bool IsLeader = false;
        public PlayerState playerState;

        public string playerId;
        public string nickName;
        public long level;
        public ushort startStage;
        public Dictionary<string, string> options = new Dictionary<string, string>();

        public PlayerInfo()
        {
            level = 0;
            playerState = PlayerState.None;
        }

        public PlayerInfo(PlayerInfo playerInfo)
        {
            IsLeader = playerInfo.IsLeader;
            playerState = playerInfo.playerState;
            playerId = playerInfo.playerId;
            nickName = playerInfo.nickName;
            level = playerInfo.level;
            startStage = playerInfo.startStage;
            options = playerInfo.options;
        }

        public PlayerInfo(Player player, bool IsLeader, PlayerState playerState)
        {
            this.IsLeader = IsLeader;
            playerId = player.PlayerId;
            nickName = player.NickName;
            level = player.Level;
            this.playerState = playerState;
        }

        public void ChangePlayerState(PlayerState playerState)
        {
            this.playerState = playerState;
        }

        public void ChangeStartStage(ushort startStage)
        {
            this.startStage = startStage;
        }

        public void TogglePlayerReady()
        {
            if (PlayerState.None.Equals(playerState))
            {
                playerState = PlayerState.Ready;
            }
            else if (PlayerState.Ready.Equals(playerState))
            {
                playerState = PlayerState.None;
            }
        }
    }
}
