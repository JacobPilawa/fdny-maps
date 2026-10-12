import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Executes Axe press/deadline and held root handlers, stubbing child behavior. */
public class HoseAxeActionRootPcodeOracle extends GhidraScript {
 private static final long AXE=0x20010000L,PLAYER=0x20020000L,PRIMARY=AXE+0x200,SECONDARY=AXE+0x648;
 private static final long VTP=0x20050000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long CHILD_UPDATE=0x00460920L,REPARENT=0x00483a20L;
 private static final long CLOCK=0x00502afcL,DAT_FLAG=0x00502e04L;
 private EmulatorHelper emu;private final List<String> events=new ArrayList<>();
 private boolean primaryRaises,secondaryRaises;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void put8(long p,int v)throws Exception{emu.writeMemory(a(p),new byte[]{(byte)v});}
 private void put32(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int get8(long p)throws Exception{return emu.readMemory(a(p),1)[0]&255;}
 private int get32(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void ret(int args)throws Exception{long sp=emu.readRegister("ESP").longValue();int target=get32(sp);emu.writeRegister("EAX",0);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(target));}
 private void init()throws Exception{
  emu=new EmulatorHelper(currentProgram);events.clear();
  emu.writeMemory(a(AXE),new byte[0xb00]);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
  emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  put32(PRIMARY,(int)VTP);put32(SECONDARY,(int)VTP);put32(PRIMARY+0x54,(int)PLAYER);put32(SECONDARY+0x54,(int)PLAYER);put32(VTP+0x18,(int)CHILD_UPDATE);put32(AXE+0x254,(int)PLAYER);
 }
 private void runToReturn(long entry,int thisPtr,int arg,int argc)throws Exception{
  put32(STACK,(int)RETURN);if(argc>0)put32(STACK+4,arg);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",thisPtr);emu.getEmulator().setExecuteAddress(entry);
  for(int steps=0;steps<10000;steps++){
   long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)return;
   if(pc==CHILD_UPDATE){long receiver=emu.readRegister("ECX").longValue();boolean primary=receiver==PRIMARY;events.add(primary?"child:primary:00460920":"child:secondary:00460920");if(primary?primaryRaises:secondaryRaises)put8(DAT_FLAG,1);ret(0);continue;}
   if(pc==REPARENT){long receiver=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();int frame=get32(sp+4),flag=get32(sp+8);events.add("reparent:"+(receiver-AXE)+":"+(Integer.toUnsignedLong(frame)-PLAYER)+":"+flag);ret(8);continue;}
   if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
  }
  throw new IllegalStateException("instruction limit at "+emu.getExecutionAddress());
 }
 private String press(String name,int clock)throws Exception{
  init();put32(CLOCK,clock);runToReturn(0x0045f8f0L,(int)AXE,0,0);
  int deadline=get32(AXE+0xa90);String row="{\"name\":\""+name+"\",\"kind\":\"press\",\"clock\":"+clock+",\"deadline\":"+deadline+"}";emu.dispose();return row;
 }
 private String held(String name,int clock,int deadline,boolean primaryEnabled,boolean primaryFlag,boolean secondaryFlag)throws Exception{
  init();primaryRaises=primaryFlag;secondaryRaises=secondaryFlag;put32(CLOCK,clock);put32(AXE+0xa90,deadline);put8(AXE+0x1fc,primaryEnabled?1:0);put8(DAT_FLAG,1);
  runToReturn(0x0045fad0L,(int)AXE,0,0);
  String row="{\"name\":\""+name+"\",\"kind\":\"held\",\"clock\":"+clock+",\"deadline\":"+deadline+",\"primary_enabled\":"+(primaryEnabled?1:0)+",\"primary_flag\":"+(primaryFlag?1:0)+",\"secondary_flag\":"+(secondaryFlag?1:0)+",\"primary_state\":"+get32(AXE+0x568)+",\"primary_timer_bits\":"+get32(AXE+0x53c)+",\"secondary_state\":"+get32(AXE+0x9b0)+",\"secondary_timer_bits\":"+get32(AXE+0x984)+",\"dat_flag\":"+get8(DAT_FLAG)+",\"events\":[\""+String.join("\",\"",events)+"\"]}";emu.dispose();return row;
 }
 @Override public void run()throws Exception{
  String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseAxeActionRootPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String> rows=new ArrayList<>();rows.add(press("press_clock_zero",0));rows.add(press("press_wrap",-50));
  rows.add(held("held_equal_deadline",100,100,true,true,true));rows.add(held("held_before_deadline",99,100,true,true,false));rows.add(held("held_after_deadline_no_hit",101,100,true,false,false));rows.add(held("held_secondary_flag_primary_disabled",101,100,false,false,true));rows.add(held("held_primary_flag_only",101,100,true,true,false));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entries\":[\"0045F8F0\",\"0045FAD0\"],\"child_plus18_target\":\"00460920\",\"scope\":\"Real Axe root press and held handlers. Child +0x18 behavior at 00460920 is stubbed with controlled DAT_00502E04 writes; only follower reparent 00483A20 is stubbed. No strike behavior is modeled.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" Axe root callback cases");
 }
}
