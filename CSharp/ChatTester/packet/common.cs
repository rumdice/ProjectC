using Newtonsoft.Json;
using System;

namespace CommonPacketStruct
{
    public enum ErrorCode : int
    {
        Success = 0,

        InvalidSession = 1,
        InvalidParam = 2,
        MissingParam = 3,


        DBError = 100,
        InternalError = 200,

        Undefinded = -1,
    }

    public abstract class BaseRequest
    {
        [JsonIgnore]
        public virtual string URL => "";
    }

    public class BaseResponse
    {
        public ErrorCode error = ErrorCode.Undefinded;
    }
}
