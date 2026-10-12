import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute one Axe child vtable +0x20 callback (004610E0) with bounded services. */
public class HoseAxeChildThinkPcodeOracle extends GhidraScript {
 private static final long CHILD=0x20010000L,PLAYER=0x20020000L,PLAYER_VT=0x20050000L;
 private static final long PLAYER_FRAME=0x20060000L,MODEL_ARRAY=0x20068000L,MODEL0=0x20070000L,MODEL1=0x20071000L,MODEL2=0x20072000L,FRAME0=0x20080000L,FRAME1=0x20081000L;
 private static final long USER=0x20030000L,GLOBAL=0x20090000L,SCENE=0x200a0000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long ENTRY=0x004610e0L,STRIKE=0x00460b90L,RECOVERY=0x0045fa60L,SCALE=0x0045fec0L,GETTER=0x20050f00L,MODEL_UPDATE=0x0047f960L;
 private EmulatorHelper emu; private final List<String> events=new ArrayList<>();
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private void w8(long p,int v)throws Exception{emu.writeMemory(a(p),new byte[]{(byte)v});}
 private void wFloat(long p,float v)throws Exception{w(p,Float.floatToRawIntBits(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private int b(long p)throws Exception{return emu.readMemory(a(p),1)[0]&255;}
 private float f(long p)throws Exception{return Float.intBitsToFloat(r(p));}
 private double d(long p)throws Exception{byte[]b=emu.readMemory(a(p),8);long v=0;for(int i=0;i<8;i++)v|=(long)(b[i]&255)<<(8*i);return Double.longBitsToDouble(v);}
 private void ret(int args,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();int target=r(sp);emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(target));}
 private void init()throws Exception{
  emu=new EmulatorHelper(currentProgram);events.clear();
  emu.writeMemory(a(CHILD),new byte[0x500]);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(PLAYER_VT),new byte[0x200]);emu.writeMemory(a(PLAYER_FRAME),new byte[0x100]);emu.writeMemory(a(USER),new byte[0x100]);emu.writeMemory(a(GLOBAL),new byte[0x100]);emu.writeMemory(a(MODEL_ARRAY),new byte[0x100]);emu.writeMemory(a(MODEL0),new byte[0x300]);emu.writeMemory(a(MODEL1),new byte[0x300]);emu.writeMemory(a(MODEL2),new byte[0x300]);emu.writeMemory(a(FRAME0),new byte[0x300]);emu.writeMemory(a(FRAME1),new byte[0x300]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  w(CHILD+0x54,(int)PLAYER);w(CHILD+0xec,(int)USER);w(CHILD+0x324,(int)MODEL_ARRAY);w(CHILD+0x328,0);w(USER+0x80,0x7f);
  w(PLAYER,(int)PLAYER_VT);w(PLAYER_VT+0x18,(int)GETTER);w(PLAYER_FRAME+0x28,0);w(PLAYER_FRAME+0x2c,0);w(PLAYER_FRAME+0x30,0);
  w(0x00503a78L,(int)GLOBAL);w(GLOBAL+4,(int)SCENE);
 }
 private long floatBits(long p)throws Exception{return Integer.toUnsignedLong(r(p));}
 private String eventsJson(){List<String> q=new ArrayList<>();for(String e:events)q.add("\""+e+"\"");return "["+String.join(",",q)+"]";}
 private String modelsJson(int count)throws Exception{
  List<String> rows=new ArrayList<>();long[] models={MODEL0,MODEL1,MODEL2};
  for(int i=0;i<count;i++)rows.add("{\"model\":"+Long.toUnsignedString(models[i])+",\"frame_present\":"+(r(models[i]+0x168)!=0?1:0)+",\"frame_time_bits\":"+floatBits(r(models[i]+0x168)!=0?r(models[i]+0x168)+0x1d8:models[i]+0x16c)+",\"model_time_bits\":"+floatBits(models[i]+0x16c)+",\"enabled\":"+b(models[i]+0x30)+"}");
  return "["+String.join(",",rows)+"]";
 }
 private String sample(String name,boolean active,int state,float[] childPos,float[] playerPos,boolean modelCase)throws Exception{
  init();w8(PLAYER+0x345c,0x7f);w8(CHILD+0x158,active?1:0);w(CHILD+0x368,state);
  if(state==2){float threshold=f(0x004e1604L);if(name.equals("state2_distance_equal"))playerPos=new float[]{Math.abs(threshold),0,0};else if(name.equals("state2_distance_far"))playerPos=new float[]{Math.abs(threshold)+10.0f,0,0};}
  for(int i=0;i<3;i++){wFloat(CHILD+0x39c+i*4,childPos[i]);wFloat(PLAYER_FRAME+0x28+i*4,playerPos[i]);}
  int count=0;
  if(modelCase){count=3;w(CHILD+0x328,count);long[]models={MODEL0,MODEL1,MODEL2};w(MODEL_ARRAY,(int)MODEL0);w(MODEL_ARRAY+4,(int)MODEL1);w(MODEL_ARRAY+8,(int)MODEL2);
   w8(MODEL0+0x30,0x7f);w(MODEL1+0x30,0x7f);w(MODEL2+0x30,0x7f);
   w(MODEL0+0x168,0);w(MODEL1+0x168,(int)FRAME0);w(MODEL2+0x168,(int)FRAME1);
   wFloat(MODEL0+0x16c,0);wFloat(MODEL1+0x16c,0);wFloat(MODEL2+0x16c,2);
   double threshold=d(0x004dfc78L);float far=(float)(Math.abs(threshold)*4.0+1.0);wFloat(FRAME0+0x1d8,far);wFloat(FRAME1+0x1d8,2.0f);
  }
  w(STACK,(int)RETURN);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",CHILD);emu.getEmulator().setExecuteAddress(ENTRY);
  for(int steps=0;steps<50000;steps++){
   long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;
   if(pc==STRIKE){events.add("strike:00460B90");ret(0,0);continue;}
   if(pc==RECOVERY){long recv=emu.readRegister("ECX").longValue();int arg=r(emu.readRegister("ESP").longValue()+4);events.add("recovery:0045FA60:"+(recv-CHILD)+":"+Integer.toUnsignedString(arg));ret(4,0);continue;}
   if(pc==SCALE){long recv=emu.readRegister("ECX").longValue();int arg=r(emu.readRegister("ESP").longValue()+4);events.add("scale:0045FEC0:"+(recv-CHILD)+":"+Integer.toUnsignedString(arg));ret(4,0);continue;}
   if(pc==GETTER){events.add("player_getter:vtable+18");ret(0,PLAYER_FRAME);continue;}
   if(pc==MODEL_UPDATE){long recv=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();events.add("model_update:0047F960:"+(recv-MODEL0)+":"+Integer.toUnsignedString(r(sp+4)));ret(4,0);continue;}
   if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
  }
  if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("004610E0 instruction limit at "+emu.getExecutionAddress());
  return "{\"name\":\""+name+"\",\"active\":"+(active?1:0)+",\"state\":"+state+",\"child_position\":["+childPos[0]+","+childPos[1]+","+childPos[2]+"],\"player_position\":["+playerPos[0]+","+playerPos[1]+","+playerPos[2]+"],\"distance_threshold_bits\":"+floatBits(0x004e1604L)+",\"model_threshold_bits\":"+Long.toUnsignedString(Double.doubleToRawLongBits(d(0x004dfc78L)))+",\"model_update_argument\":"+Integer.toUnsignedString(r(GLOBAL+4))+",\"models\":"+modelsJson(count)+",\"player_345c_before\":127,\"player_345c\":"+b(PLAYER+0x345c)+",\"actor_flag80\":"+b(USER+0x80)+",\"model_enabled\":["+b(MODEL0+0x30)+","+b(MODEL1+0x30)+","+b(MODEL2+0x30)+"],\"events\":"+eventsJson()+"}";
 }
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseAxeChildThinkPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String> rows=new ArrayList<>();rows.add(sample("inactive_state1",false,1,new float[]{0,0,0},new float[]{0,0,0},false));rows.add(sample("active_idle",true,0,new float[]{0,0,0},new float[]{0,0,0},false));rows.add(sample("active_state1_strike_boundary",true,1,new float[]{0,0,0},new float[]{0,0,0},false));rows.add(sample("state2_distance_equal",true,2,new float[]{0,0,0},new float[]{0,0,0},false));rows.add(sample("state2_distance_far",true,2,new float[]{0,0,0},new float[]{0,0,0},false));rows.add(sample("state3_scale_boundary",true,3,new float[]{0,0,0},new float[]{0,0,0},false));rows.add(sample("active_model_timer_pass",true,0,new float[]{0,0,0},new float[]{0,0,0},true));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"004610E0\",\"scope\":\"One controlled child vtable +0x20 call only. Original body 004610E0 executes. Strike 00460B90, recovery 0045FA60, visual scale 0045FEC0, Player getter vtable +0x18, and per-model 0047F960 are intercepted boundaries; model timer comparison and stores execute original P-code. No caller cadence, state advancement by the stubbed callees, strike geometry, world query, sound/stats, or live Training values are covered.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" bounded Axe child Think cases");
 }
}
