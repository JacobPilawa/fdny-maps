import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute the original post-status burn/indicator slice of Player 00452570. */
public class PlayerBurnCoolingPcodeOracle extends GhidraScript {
 private static final long PLAYER=0x20000000L,ESI=PLAYER+0x140,STACK=0x20fff000L;
 private static final long COPY=0x00441e90L,STOP=0x00452a5fL;
 private EmulatorHelper emu;
 private int copyCalls,clampCalls;
 private long copyReceiverOffset,copySourceOffset,copyBurnBits,preClampBits;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void b(long p,int v)throws Exception{emu.writeMemory(a(p),new byte[]{(byte)v});}
 private static int fb(float f){return Float.floatToRawIntBits(f);}
 private static String words(int[]v){StringBuilder s=new StringBuilder("[");for(int i=0;i<v.length;i++){if(i>0)s.append(',');s.append(Integer.toUnsignedString(v[i]));}return s.append(']').toString();}
 private void init(float accumulator,float dt,int flagIndex,int sideFlag,float sideThreshold,int sourceSeed,int targetSeed)throws Exception{
  emu=new EmulatorHelper(currentProgram);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(STACK),new byte[0x100]);
  for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(n,0);
  emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)emu.writeRegister(cw,0x037fL);
  Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)emu.writeRegister(sw,0);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)emu.writeRegister(tag,0xffffL);
  for(Register reg:currentProgram.getLanguage().getRegisters())if(reg.getName().toUpperCase().matches("ST[0-7]"))emu.writeRegister(reg,BigInteger.ZERO);
  w(PLAYER+0x3538,fb(accumulator));w(0x00502b04L,fb(dt));b(PLAYER+0x12c,sideFlag);w(PLAYER+0x3450,fb(sideThreshold));b(PLAYER+0x5d4,0xa5);
  for(int i=0;i<4;i++)b(PLAYER+0x353c+i,i==flagIndex?1:0);
  for(int i=0;i<3;i++){w(PLAYER+0x4c+i*4,sourceSeed+i*0x10101);w(PLAYER+0x3400+i*4,targetSeed+i*0x20202);}
  emu.writeRegister("ESI",ESI);emu.writeRegister("ESP",STACK);emu.getEmulator().setExecuteAddress(0x004529a2L);
  copyCalls=clampCalls=0;copyReceiverOffset=copySourceOffset=copyBurnBits=preClampBits=0;
 }
 private String sample(String name,float accumulator,float dt,int flagIndex,int sideFlag,float sideThreshold,int sourceSeed,int targetSeed)throws Exception{
  init(accumulator,dt,flagIndex,sideFlag,sideThreshold,sourceSeed,targetSeed);
  try{
   for(int steps=0;steps<1000;steps++){
    long pc=emu.getExecutionAddress().getOffset();if(pc==STOP)break;
    if(pc==0x00452a22L)preClampBits=Integer.toUnsignedLong(r(PLAYER+0x3538));
    if(pc==0x00452a35L)clampCalls++;
    if(pc==COPY){
     copyCalls++;copyReceiverOffset=emu.readRegister("ECX").longValue()-PLAYER;
     long sp=emu.readRegister("ESP").longValue();copySourceOffset=Integer.toUnsignedLong(r(sp+4))-PLAYER;
     copyBurnBits=Integer.toUnsignedLong(r(PLAYER+0x3538));
    }
    if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(emu.getExecutionAddress().getOffset()!=STOP)throw new IllegalStateException("Cooling instruction limit at "+emu.getExecutionAddress());
   int[]flags=new int[4],source=new int[3],target=new int[3],targetBefore=new int[3];
   for(int i=0;i<4;i++)flags[i]=r(PLAYER+0x353c+i)&255;
   for(int i=0;i<3;i++){source[i]=r(PLAYER+0x4c+i*4);target[i]=r(PLAYER+0x3400+i*4);targetBefore[i]=targetSeed+i*0x20202;}
   return "{\"name\":\""+name+"\",\"accumulator_before_bits\":"+Integer.toUnsignedString(fb(accumulator))+
     ",\"dt_bits\":"+Integer.toUnsignedString(fb(dt))+
     ",\"flags_before\":"+words(new int[]{flagIndex==0?1:0,flagIndex==1?1:0,flagIndex==2?1:0,flagIndex==3?1:0})+
     ",\"side_flag_before\":"+sideFlag+",\"side_threshold_before_bits\":"+Integer.toUnsignedString(fb(sideThreshold))+
     ",\"status_marker_before\":165,\"status_marker_after\":"+(r(PLAYER+0x5d4)&255)+
     ",\"source_words_before\":"+words(new int[]{sourceSeed,sourceSeed+0x10101,sourceSeed+0x20202})+
     ",\"target_words_before\":"+words(targetBefore)+",\"accumulator_after_bits\":"+Integer.toUnsignedString(r(PLAYER+0x3538))+
     ",\"cooling_applied\":"+(flagIndex<0)+",\"pre_clamp_bits\":"+preClampBits+",\"clamp_calls\":"+clampCalls+
     ",\"flags_after\":"+words(flags)+",\"copy_calls\":"+copyCalls+
     ",\"copy_receiver_offset\":"+copyReceiverOffset+",\"copy_source_offset\":"+copySourceOffset+
     ",\"copy_accumulator_bits\":"+copyBurnBits+",\"source_words_after\":"+words(source)+",\"target_words_after\":"+words(target)+"}";
  }finally{emu.dispose();}
 }
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: PlayerBurnCoolingPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String>rows=new ArrayList<>();
  rows.add(sample("cool_positive",0.4f,0.5f,-1,0,0.75f,0x11223344,0x55667788));
  for(int i=0;i<4;i++)rows.add(sample("indicator_blocks_cooling_"+i,0.4f,0.5f,i,0,0.75f,0x11223344+i,0x55667788+i));
  rows.add(sample("below_point_five_triggers_copy",0.15f,1.0f,-1,1,0.5001f,0x12345670,0x76543210));
  rows.add(sample("negative_copy_before_clamp",0.05f,1.0f,-1,1,0.75f,0x10203040,0x50607080));
  rows.add(sample("side_flag_false",0.05f,1.0f,-1,0,0.75f,0x10203040,0x50607080));
  rows.add(sample("threshold_equal_half",0.05f,1.0f,-1,1,0.5f,0x10203040,0x50607080));
  rows.add(sample("negative_with_indicator_clamps",-0.25f,1.0f,2,1,0.75f,0x10203040,0x50607080));
  rows.add(sample("adjacent_below_point_one_with_zero_dt",Math.nextDown(0.1f),0.0f,-1,1,0.75f,0x22334450,0x66778890));
  rows.add(sample("float_point_one_above_binary64_threshold",0.1f,0.0f,-1,1,0.75f,0x22334450,0x66778890));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"004529A2 (00452570 after 004559D0 status service and non-state-6 dispatch)\",\"stop\":\"00452A5F before the following movement branch\",\"scope\":\"Original indicator-gated x87 burn subtraction, non-pop binary32 store and binary64 0.1 comparison; side-call gates and original 00441E90 three-dword copy execute; negative clamp and four flag clears; excludes status service, state-6 00451080, following movement logic and recurring schedule\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original Player burn-cooling cases");
 }
}
