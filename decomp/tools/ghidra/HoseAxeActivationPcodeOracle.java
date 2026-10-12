import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute Training Axe +0x3C activation/deactivation and both child callbacks. */
public class HoseAxeActivationPcodeOracle extends GhidraScript {
 private static final long AXE=0x20010000L,PLAYER=0x20020000L,PRIMARY=AXE+0x200,SECONDARY=AXE+0x648;
 private static final long VTP=0x20050000L,VTS=0x20050100L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long ANIMATION=0x00497de0L,REPARENT=0x00483a20L;
 private EmulatorHelper emu;private final List<String> events=new ArrayList<>(),animationSnapshots=new ArrayList<>();
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private int b(long p)throws Exception{return emu.readMemory(a(p),1)[0]&255;}
 private void ret(int args)throws Exception{long sp=emu.readRegister("ESP").longValue();int target=r(sp);emu.writeRegister("EAX",0);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(target));}
 private String sample(String name,boolean active,boolean primaryEnabled)throws Exception{
  emu=new EmulatorHelper(currentProgram);events.clear();animationSnapshots.clear();
  try{
   emu.writeMemory(a(AXE),new byte[0x700]);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
   for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
   w(AXE+0x54,(int)PLAYER);w(AXE+0x1fc,primaryEnabled?1:0);
   w(PRIMARY,(int)VTP);w(PRIMARY+0x54,(int)PLAYER);w(SECONDARY,(int)VTS);w(SECONDARY+0x54,(int)PLAYER);
   w(VTP+0x3c,0x0045fa30);w(VTS+0x3c,0x0045fa30);w(PLAYER+0x345c,0x7f);
   w(STACK,(int)RETURN);w(STACK+4,active?1:0);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",AXE);emu.getEmulator().setExecuteAddress(0x0045f9b0L);
   for(int steps=0;steps<10000;steps++){
    long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;
    if(pc==ANIMATION){long receiver=emu.readRegister("ECX").longValue();int arg=r(emu.readRegister("ESP").longValue()+4),activeByte=b(receiver-0x58+0x158),playerByte=b(PLAYER+0x345c);events.add("animation:"+(receiver-AXE)+":"+arg+":0");animationSnapshots.add("animation-state:"+(receiver-AXE)+":"+activeByte+":"+playerByte);ret(4);continue;}
    if(pc==REPARENT){long receiver=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();int frame=r(sp+4),flag=r(sp+8);events.add("reparent:"+(receiver-AXE)+":"+(Integer.toUnsignedLong(frame)-PLAYER)+":"+flag);ret(8);continue;}
    if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Axe callback instruction limit at "+emu.getExecutionAddress());
   return "{\"name\":\""+name+"\",\"active\":"+(active?1:0)+",\"primary_enabled\":"+(primaryEnabled?1:0)+",\"axe_active\":"+b(AXE+0x158)+",\"primary_active\":"+b(PRIMARY+0x158)+",\"secondary_active\":"+b(SECONDARY+0x158)+",\"player_345c\":"+b(PLAYER+0x345c)+",\"events\":[\""+String.join("\",\"",events)+"\"],\"animation_snapshots\":[\""+String.join("\",\"",animationSnapshots)+"\"]}";
  }finally{emu.dispose();}
 }
 @Override public void run()throws Exception{
  String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseAxeActivationPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String> rows=new ArrayList<>();rows.add(sample("activate_primary_disabled",true,false));rows.add(sample("activate_primary_enabled",true,true));rows.add(sample("deactivate_primary_disabled",false,false));rows.add(sample("deactivate_primary_enabled",false,true));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entries\":[\"0045F9B0\",\"0045F620\",\"0045FA30\"],\"scope\":\"Training Axe vtable +0x3C callback with both child +0x3C routes executing real 0045FA30. Only animation 00497DE0 and follower reparent 00483A20 services are intercepted; receiver offsets, arguments, callback-visible active bytes and final writes are recorded.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" Axe activation callbacks");
 }
}
