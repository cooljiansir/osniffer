var Packages = new Array();

function allPrpos(obj, func) {
    var props = new Array();
    for (var p in obj) {
        var a=typeof(obj[p]);
        if (typeof(obj[p]) == "object" && obj[p].end && !obj[p].name) {//报文才有name
            var o=new Array();
            o=allPrpos(obj[p],func);
            props.push(func( p, obj[p],o));
        }
    }
    return props;
}

function _byte(s, b, e) { //16进制->指定有效位2进制 ..1. ....
    var t = parseInt(s, 16);
    var type = "";
    for (var i = 0; i < 8; ++i) {
        if (i == 4) type += " ";
        if (i >= b && i <= e) type += (t >> (7 - i)) & 1;
        else type += ".";
    }
    return type;
}

function _field(b, e, s,h) {
    if(b<e)
        return {
            beg: b,
            end: e,
            res: s,
            head: h
        };
    return new Object();
}

function _protocal(name, beg, end) {
    this.name = name;
    this.beg = beg;
    this.end = end;
    this.upperProtocal = function() {
        return new Object();
    };
    Packages.push(this);
}
function _group(res,beg,end){
    this.res = res;
    this.beg = beg;
    this.end = end;
}

function Ethernet(argv) {
    var data = argv;
    this.Destination = (function() {
        var mac = data[0];
        for (var i = 1; i < 12; ++i) {
            if (i % 2 === 0) mac += ":";
            mac += data[i];
        }
        return _field( 0, 12, mac);
    })();
    this.Source = (function() {
        var mac = data[12];
        for (var i = 13; i < 24; ++i) {
            if (i % 2 === 0) mac += ":";
            mac += data[i];
        }
        return _field( 12, 24, mac);
    })();
    this.Type = (function() {
        var type = data[24] + data[25] + data[26] + data[27];
        if (type === "0800") type = "IP";
        else if (type === "0801") type = "X.75 Internet";
        else if (type === "0802") type = "NBS Internet";
        else if (type === "0803") type = "ECMA Internet";
        else if (type === "0804") type = "Chaosnet";
        else if (type === "0805") type = "X.25 Level 3";
        else if (type === "0806") type = "ARP ： Address Resolution Protocol";
        else if (type === "0808") type = "Frame Relay ARP";
        else if (type === "6559") type = "Raw Frame Relay";
        else if (type === "8035") type = "DRARP：Dynamic RARP";
        else if (type === "8037") type = "Novell Netware IPX";
        else if (type === "809b") type = "EtherTalk";
        else if (type === "80d5") type = "IBM SNA Services over Ethernet";
        else if (type === "80f3") type = "AARP：AppleTalk Address Resolution Protocol";
        else if (type === "8100") type = "EAPS：Ethernet Automatic Protection Switching";
        else if (type === "8137") type = "IPX：Internet Packet Exchange";
        else if (type === "814c") type = "SNMP：Simple Network Management Protocol";
        else if (type === "86dd") type = "IPv6，Internet Protocol version 6";
        else if (type === "8809") type = "OAM";
        else if (type === "880b") type = "PPP：Point-to-Point Protocol";
        else if (type === "880c") type = "GSMP：General Switch Management Protocol";
        else if (type === "8847") type = "MPLS：Multi-Protocol Label Switching ";
        else if (type === "8848") type = "MPLS";
        else if (type === "8863") type = "PPPoE：PPP Over Ethernet <Discovery Stage>";
        else if (type === "8864") type = "PPPoE，PPP Over Ethernet<PPP Session Stage>";
        else if (type === "88bb") type = "LWAPP：Light Weight Access Point Protocol";
        else if (type === "88cc") type = "LLDP：Link Layer Discovery Protocol";
        else if (type === "8e88") type = "EAP（EAPOL：EAP over LAN";
        else if (type === "9000") type = "Loopback";
        else if (type === "9100") type = "VLAN Tag Protocol Identifier";
        else if (type === "9200") type = "VLAN Tag Protocol Identifier";
        return _field( 24, 28, type);
    })();

    _protocal.call(this, "Ethernet", 0, 28);
    this.res = "from " + this.Source.res + " to " + this.Destination.res + " Pro:" + this.Type.res + "(" + this.Type.res + ")";
    this.upperProtocal = function() {
        var type = data[24] + data[25] + data[26] + data[27];
        if (type === "0806") return new ARP(data);
        else if (type === "0800") return new IPv4(data);
        else if (type === "8864") return new PPPoE(data);
        else if (type === "86dd") return new IPv6(data);
        else if (type === "8847") return new MPLS(data);
        // TODO
        return Object();
    };
}

function ARP(argv) {
    var data = argv;
    this.Hardware = (function() {
        var type = data[28] + data[29] + data[30] + data[31];
        if (type === "0000") type = "reserved";
        else if (type === "0001") type = "ethernet";
        else if (type === "0002") type = "experimental ethernet";
        else if (type === "0003") type = "amateur radio ax.25";
        else if (type === "0004") type = "proteon pronet token ring";
        else if (type === "0005") type = "chaos";
        else if (type === "0006") type = "ieee 802";
        else if (type === "0007") type = "arcnet";
        else if (type === "0008") type = "hyperchannel";
        else if (type === "0009") type = "lanstar";
        else if (type === "000a") type = "autonet short address";
        else if (type === "000b") type = "localtalk";
        else if (type === "000c") type = "localnet (ibm pcnet or sytek localnet)";
        else if (type === "000d") type = "ultra link";
        else if (type === "000e") type = "smds";
        else if (type === "000f") type = "frame relay";
        else if (type === "0010") type = "atm, asynchronous transmission mode";
        else if (type === "0011") type = "hdlc";
        else if (type === "0012") type = "fibre channel";
        else if (type === "0013") type = "atm, asynchronous transmission mode";
        else if (type === "0014") type = "serial line";
        else if (type === "0015") type = "atm, asynchronous transmission mode";
        else if (type === "0016") type = "mil-std-188-220";
        else if (type === "0017") type = "metricom";
        else if (type === "0018") type = "ieee 1394.1995";
        else if (type === "0019") type = "mapos";
        else if (type === "001a") type = "twinaxial";
        else if (type === "001b") type = "eui-64";
        else if (type === "001c") type = "hiparp";
        else if (type === "001d") type = "ip and arp over iso 7816-3";
        else if (type === "001e") type = "arpsec";
        else if (type === "001f") type = "ipsec tunnel";
        else if (type === "0020") type = "infiniband";
        else if (type === "0021") type = "cai, tia-102 project 25 common air interface";
        else if (type === "0022") type = "wiegand interface";
        else if (type === "0023") type = "pure ip";
        else if (type === "0024") type = "hw_exp1";
        else if (type === "0100") type = "hw_exp2";
        else if (type === "ffff") type = "reserved";
        return _field( 28, 32, type);
    })();
    this.Protocal = (function() {
        var type = data[32] + data[33] + data[34] + data[35];
        if (type == "0800") type = "IP (0x0800)";
        return _field( 32, 36, type);
    })();
    this.Hardware_Size = (function() {
        var type = parseInt(data[36] + data[37], 16);
        return _field( 36, 38, type);
    })();
    this.Protocal_Size = (function() {
        var type = parseInt(data[38] + data[39], 16);
        return _field( 38, 40, type);
    })();
    this.Opcode = (function() {
        var type = data[40] + data[41] + data[42] + data[43];
        if (type === "0000") type = "reserved";
        else if (type === "0001") type = "request";
        else if (type === "0002") type = "reply";
        else if (type === "0003") type = "reverse request";
        else if (type === "0004") type = "reverse reply";
        else if (type === "0005") type = "request";
        else if (type === "0006") type = "drarp reply";
        else if (type === "0007") type = "drarp reply";
        else if (type === "0008") type = "drarp error";
        else if (type === "0009") type = "inverse request";
        else if (type === "000a") type = "inverse reply";
        else if (type === "000b") type = "arp nak";
        else if (type === "000c") type = "mars request";
        else if (type === "000d") type = "mars multi";
        else if (type === "000e") type = "mars mserv";
        else if (type === "000f") type = "mars join";
        else if (type === "0010") type = "mars leave";
        else if (type === "0011") type = "mars nak";
        else if (type === "0012") type = "mars unserv";
        else if (type === "0013") type = "mars sjoin";
        else if (type === "0014") type = "mars sleave";
        else if (type === "0015") type = "mars grouplist";
        else if (type === "0016") type = "mars grouplist";
        else if (type === "0017") type = "mars redirect";
        else if (type === "0018") type = "mapos unarp";
        else if (type === "0019") type = "op_exp1";
        else if (type === "0020") type = "op_exp2";
        return _field( 40, 44, type);
    })();
    this.Sender_Mac = (function() {
        var mac = data[44];
        for (var i = 45; i < 56; ++i) {
            if (i % 2 === 0) mac += ":";
            mac += data[i];
        }
        return _field( 44, 56, mac);
    })();
    this.Sender_IP = (function() {
        var ip = parseInt(data[56] + data[57], 16);
        for (var i = 58; i < 64; i += 2) {
            ip += "." + parseInt(data[i] + data[i + 1], 16);
        }
        return _field( 56, 64, ip);
    })();
    this.Target_Mac = (function() {
        var mac = data[64];
        for (var i = 65; i < 76; ++i) {
            if (i % 2 === 0) mac += ":";
            mac += data[i];
        }
        return _field( 64, 76, mac);
    })();
    this.Target_IP = (function() {
        var ip = parseInt(data[76] + data[77], 16);
        for (var i = 78; i < 84; i += 2) {
            ip += "." + parseInt(data[i] + data[i + 1], 16);
        }
        return _field( 76, 84, ip);
    })();

    _protocal.call(this, "ARP", 14, 84);
    var type="";
    if (this.Opcode(true) === "0001") type= "who has " + this.TargetIP.res + "? Tell " + this.SenderIP.res;
    else if (this.Opcode(true) === "0002") type= this.SenderIP.res + " is at " + this.SenderMac.res;
    // TODO
    this.res = type;
}

function IPv4(argv) {
    var data = argv;
    this.Version = (function() {
        var type = data[28];
        if (type === "4") type = "IPv4";
        else if (type === "6") type = "IPv6";
        // TODO
        return _field( 28, 29, type);
    })();
    this.Header_Length = (function() {
        var type = parseInt(data[29], 16);
        return _field( 29, 30, type * 4);
    })();
    this.Type_Of_Service=(function(){
        var obj=new Object();
        obj.Precedence = (function() {
            var type = (parseInt(data[30], 16) >> 1);
            return _field( 30, 31, type,_byte(data[30] + data[31], 0, 2));
        })();
        obj.Low_Delay = (function() {
            var type = (parseInt(data[30], 16) & 1);
            return _field( 30, 31, type, _byte(data[30] + data[31], 3, 3));
        })();
        obj.High_Throughput = (function() {
            var type = ((parseInt(data[31], 16) >> 3) & 1) ;
            return _field( 31, 32, type,_byte(data[30] + data[31], 4, 4));
        })();
        obj.High_Ralibality = (function() {
            var type = ((parseInt(data[31], 16) >> 2) & 1);
            return _field( 31, 32, type,_byte(data[30] + data[31], 5, 5));
        })();
        obj.Low_Cost = (function() {
            var type = ((parseInt(data[31], 16) >> 1) & 1);
            return _field( 31, 32, type,_byte(data[30] + data[31], 6, 6));
        })();
        obj.Reserved = (function() {
            var type = ((parseInt(data[31], 16) >> 0) & 1);
            return _field( 31, 32, type,_byte(data[30] + data[31], 7, 7));
        })();
        _group.call(obj,"(TOS)",30,32);
        return obj;
    })();
    this.Totollength = (function() {
        var type = parseInt(data[32] + data[33] + data[34] + data[35], 16);
        return _field( 32, 36, type);
    })();
    this.Identification = (function() {
        var type = data[36] + data[37] + data[38] + data[39];
        return _field( 36, 40, type);
    })();
    this.Flags=(function(){
        var obj=new Object();
        obj.Reserved = (function() {
            var type = ((parseInt(data[40], 16) >> 3) & 1);
            return _field( 40, 41, type,_byte(data[40] + data[41], 0, 0));
        })();
        obj.Do_Not_Fragment = (function() {
            var type = ((parseInt(data[40], 16) >> 2) & 1);
            return _field( 40, 41, type,_byte(data[40] + data[41], 1, 1));
        })();
        obj.More_Fragment = (function() {
            var type = ((parseInt(data[40], 16) >> 1) & 1);
            return _field( 40, 41, type,_byte(data[40] + data[41], 2, 2));
        })();
        var type="";
        if(obj.Do_Not_Fragment.res)type="Don't Fragment";
        if(obj.More_Fragment.res)type="Fragment";
        _group.call(obj,type,40,42);
        return obj;
    })();
    this.Fragment_Offset = (function() {
        var type = parseInt((parseInt(data[40], 16) & 1) + data[41] + data[42] + data[43], 16);
        return _field( 40, 44, type);
    })();
    this.Time_To_Live = (function() {
        var type = parseInt(data[44] + data[45], 16);
        return _field( 44, 46, type);
    })();
    this.Protocal = (function() {
        var type = data[46] + data[47];
        //TODO
        if (type === "06") type = "TCP (0x06)";
        return _field( 46, 48, type);
    })();
    this.Checksum = (function() {
        var type = data[48] + data[49] + data[50] + data[51];
        return _field( 48, 52, type);
    })();
    this.Source = (function() {
        var ip = parseInt(data[52] + data[53], 16);
        for (var i = 54; i < 60; i += 2) {
            ip += "." + parseInt(data[i] + data[i + 1], 16);
        }
        return _field( 52, 60, ip);
    })();
    this.Destination = (function() {
        var ip = parseInt(data[60] + data[61], 16);
        for (var i = 62; i < 68; i += 2) {
            ip += "." + parseInt(data[i] + data[i + 1], 16);
        }
        return _field( 60, 68, ip);
    })();

    _protocal.call(this, "ip", 14, 68);
    this.res = "from " + this.Source.res + " to " + this.Destination.res + " Version:" + this.Version.res;
    this.upperProtocal = function() {
        var type = data[46] + data[47];
        if (type === "01") return new ICMP(data);
        if (type === "02") return new IGMP(data);
        if (type === "06") return new TCP(data);
        if (type === "11") return new UDP(data);
        // TODO
        return Object();
    };
}

function TCP(argv) {
    var data = argv;
    this.Source_Port = (function() {
        var type = parseInt(data[68] + data[69] + data[70] + data[71], 16);
        if (type == 80) type = "http (80)";
        return _field( 68, 72, type);
    })();
    this.Destination_Port = (function() {
        var type = parseInt(data[72] + data[73] + data[74] + data[75], 16);
        if (type == 80) type = "http (80)";
        return _field( 72, 76, type);
    })();
    this.Sequence_Number = (function() {
        // TODO 数值 
        var type = data[76] + data[77] + data[78] + data[79] + data[80] + data[81] + data[82] + data[83];
        type = parseInt(type, 16);
        return _field( 76, 84, type);
    })();
    this.Acknowledgment_Number = (function() {
        // TODO 数值 
        var type = data[84] + data[85] + data[86] + data[87] + data[88] + data[89] + data[90] + data[91];
        type = parseInt(type, 16);
        return _field( 84, 92, type);
    })();
    this.Header_Length_Flags = (function(){
        var obj=new Object();
        obj.Header_Length = (function() {
            var type = parseInt(data[92], 16) * 4;
            return _field( 92, 94, type,_byte(data[92] + data[93], 0, 3));
        })();
        obj.Reserved = (function(){
            var type = (data[93]>>3)+((data[93]>>2)&1)+(data[93]&1);
            return _field( 92, 94, type,_byte(data[92] + data[93], 4, 6));
        })();
        obj.Nonce = (function() {
            var type = parseInt(data[93], 16) & 1;
            return _field( 92, 94, type,_byte(data[92] + data[93], 7, 7));
        })();
        var type="Header Length:"+obj.Header_Length.res+" bytes";
        if(obj.Nonce.res)type+=" ,Nonce";
        _group.call(obj,type,92,94);
        return obj;
    })();
    this.Flags=(function(){
        var obj=new Object();
        obj.Congestion_Window_Reduced = (function() {
            var type = (parseInt(data[94], 16) >> 3) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],0,0));
        })();
        obj.ECN_Echo = (function() {
            var type = (parseInt(data[94], 16) >> 2) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],1,1));
        })();
        obj.Urgent = (function() {
            var type = (parseInt(data[94], 16) >> 1) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],2,2));
        })();
        obj.Acknowledgment = (function() {
            var type = (parseInt(data[94], 16) >> 0) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],3,3));
        })();
        obj.Push = (function() {
            var type = (parseInt(data[95], 16) >> 3) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],4,4));
        })();
        obj.Reset = (function() {
            var type = (parseInt(data[95], 16) >> 2) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],5,5));
        })();
        obj.Syn = (function() {
            var type = (parseInt(data[95], 16) >> 1) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],6,6));
        })();
        obj.Fin = (function() {
            var type = (parseInt(data[95], 16) >> 0) & 1;
            return _field( 94, 96, type,_byte(data[93]+data[94],7,7));
        })();
        var type="0x"+data[94]+data[95];
        if(obj.Congestion_Window_Reduced.res)type+=" ,CWR";
        if(obj.ECN_Echo.res)type+=" ,ECN";
        if(obj.Urgent.res)type+=" ,URG";
        if(obj.Acknowledgment.res)type+=" ,ACK";
        if(obj.Push.res)type+=" ,Push";
        if(obj.Reset.res)type+=" ,Reset";
        if(obj.Syn.res)type+=" ,SYN";
        if(obj.Fin.res)type+=" ,FIN";
        if(type.length>4){
            type[5]='(';
            type+=")";
        }
        _group.call(obj,type,94,96);
        return obj;
    })();
    this.Window_size_value = (function() {
        var type = data[96] + data[97] + data[98] + data[99];
        type = parseInt(type, 16);
        return _field( 96, 100, type);
    })();
    this.Checksum = (function() {
        var type = data[100] + data[101] + data[102] + data[103];
        return _field( 100, 104, type);
    })();
    var i=108,j,l,leng=108+this.Header_Length_Flags.Header_Length.res;
    this.Options=(function(){
        var obj=new Object();
        var num=0;
        while(i<leng&&i<data.length){
            var opt=data[i]+data[i+1];
            if(opt=="02"){
                obj[num++]=(function(){
                    var o=new Object();
                    o.Kind=(function(){
                        return _field(i,i+2,"MSS size (2)");
                    })();
                    o.Length=(function(){
                        return _field(i+2,i+4,parseInt(data[i+2]+data[i+3],16));
                    })();
                    var len=(o.Length.res-2)*2;
                    o.MSS_Value=(function(){
                        var type="";
                        for(j=0;j<len;++j){
                            type+=data[i+4+j];
                        }
                        return _field(i+4,i+4+j,parseInt(type,16));
                    })();
                    _group.call(o,"Maxinum segment size",i,i+4+len);
                    return o;
                })();
                i+=(obj[num-1].Length.res-2)*2+4;
            }
            else if(opt=="03"){
                obj[num++]=(function(){
                    var o=new Object();
                    o.Kind=(function(){
                        return _field(i,i+2,"Window scale");
                    })();
                    o.Length=(function(){
                        return _field(i+2,i+4,parseInt(data[i+2]+data[i+3],16));
                    })();
                    var len=(o.Length.res-2)*2;
                    _group.call(o,"TCP SACK Permitted Option",i,i+4+len);
                    return o;
                })();
                i+=(obj[num-1].Length.res-2)*2+4;
            }
            else if(opt=="04"){
                obj[num++]=(function(){
                    var o=new Object();
                    o.Kind=(function(){
                        return _field(i,i+2,"SACK Permitted");
                    })();
                    o.Length=(function(){
                        return _field(i+2,i+4,parseInt(data[i+2]+data[i+3],16));
                    })();
                    var len=(o.Length.res-2)*2;
                    o.Shift_count=(function(){
                        var type="";
                        for(j=0;j<len;++j){
                            type+=data[i+4+j];
                        }
                        return _field(i+4,i+4+j,parseInt(type,16));
                    })();
                    _group.call(o,"TCP SACK Permitted Option",i,i+4+len);
                    return o;
                })();
                i+=(obj[num-1].Length.res-2)*2+4;
            }
            else if(opt=="05"){
                obj[num++]=(function(){
                    var o=new Object();
                    o.left_edge=(function(){
                        var type=data[i+4]+data[i+5]+data[i+6]+data[i+7]+data[i+8]+data[i+9]+data[i+10]+data[i+11];
                        return _field(i+4,i+12,type);
                    })();
                    o.right_edge=(function(){
                        var type=data[i+12]+data[i+13]+data[i+14]+data[i+15]+data[i+16]+data[i+17]+data[i+18]+data[i+19];
                        return _field(i+12,i+20,type);
                    })();
                    _group.call(o,"TCP SACK Permitted Option",i,i+20);
                    return o;
                })();
                i+=20;
            }
            else if(opt=="08"){
                obj[num++]=(function(){
                    var o=new Object();
                    o.Kind=(function(){
                        return _field(i,i+2,"SACK Permitted");
                    })();
                    o.Length=(function(){
                        return _field(i+2,i+4,parseInt(data[i+2]+data[i+3],16));
                    })();
                    var len=(o.Length.res-2)*2;
                    o.Timestamp_value=(function(){
                        var type="";
                        for(j=0;j<8;++j){
                            type+=data[i+4+j];
                        }
                        return _field(i+4,i+4+j,parseInt(type,16));
                    })();
                    o.Timestamp_echo_reply=(function(){
                        var type="";
                        for(;j<len;++j){
                            type+=data[i+4+j];
                        }
                        return _field(i+12,i+12+j,parseInt(type,16));
                    })();
                    _group.call(o,"Timestamps: TSval "+o.Timestamp_value.res+", TSecr "+o.Timestamp_echo_reply.res,i,i+4+len);
                    return o;
                })();
                i+=(obj[num-1].Length.res-2)*2+4;
            }
            else if((parseInt(opt,16)&31)==1){
                obj[num++]=(function(){
                    var o=new Object();
                    o.Type=(function(){
                        var oo=new Object();
                        oo.Copy_on_fragmentation=(function(){
                            var type=parseInt(data[i],16)>>3;
                            return _field(i,i+2,type,_byte(data[i]+data[i+1],0,0));
                        })();
                        oo.Class=(function(){
                            var type=(parseInt(data[i],16)>>1)&3;
                            type="Control ("+type+")"
                            return _field(i,i+2,type,_byte(data[i]+data[i+1],1,2));
                        })();
                        oo.Number=(function(){
                            type="No-Operation (NOP) (1)";
                            return _field(i,i+2,type,_byte(data[i]+data[i+1],3,7));
                        })();
                        _group.call(oo,1,i,i+2);
                        return oo;
                    })();
                    _group.call(o,"No-Operation (NOP)",i,i+2);
                    return o;
                })();
                i+=2;
            }else{
                break;
            }
        }
        _group.call(obj,"( "+((i-108)/2)+" bytes)",108,i);
        if(i>108)
            return obj;
        return new Object();
    })();
    _protocal.call(this, "TCP", 68, i);
    var type = "Src Port: " + this.Source_Port.res + ", Dst Port: " + this.Destination_Port.res + ", Seq: " + this.Sequence_Number.res;
    if (this.Flags.Acknowledgment.res == 1) type += ", Ack: " + this.Acknowledgment_Number.res;
    type += ", Len: " + (data.length - 132);
    this.res = type;
}