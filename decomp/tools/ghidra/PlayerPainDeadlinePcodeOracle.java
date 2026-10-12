import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute the original due/deadline/stat/exposure suffix of Player 00451640. */
public class PlayerPainDeadlinePcodeOracle extends GhidraScript {
 private static final long PLAYER=0x20000000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long PAIN=0x004897c0L,STRING=0x00402790L,STAT=0x00486980L,RNG=0x00404ac0L;
 private EmulatorHelper emu;
 private int painCalls,statCalls,rngCalls,stringCalls;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void ret(int args,int eax)throws Exception{
  long sp=emu.readRegister("ESP").longValue();long target=Integer.toUnsignedLong(r(sp));
  emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(target);
 }
 private void seed(int n)throws Exception{
  w(STACK,(int)RETURN);w(STACK+4,n);emu.writeRegister("ESP",STACK);emu.getEmulator().setExecuteAddress(0x00404b50L);
  for(int i=0;i<20000&&emu.getExecutionAddress().getOffset()!=RETURN;i++)if(!emu.step(monitor))throw new IllegalStateException("RNG seed P-code failure: "+emu.getLastError());
  if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("RNG seed step limit");
 }
 private void init(int now,int deadline,float dt,float influence,float exposure,int seed)throws Exception{
  emu=new EmulatorHelper(currentProgram);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(STACK),new byte[0x100]);
  for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(n,0);
  emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)emu.writeRegister(cw,0x037fL);
  Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)emu.writeRegister(sw,0);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)emu.writeRegister(tag,0xffffL);
  for(Register reg:currentProgram.getLanguage().getRegisters())if(reg.getName().toUpperCase().matches("ST[0-7]"))emu.writeRegister(reg,BigInteger.ZERO);
  w(0x00502afcL,now);w(0x00502b04L,Float.floatToRawIntBits(dt));w(PLAYER+0x390,deadline);w(PLAYER+0x3538,Float.floatToRawIntBits(exposure));
  // This is the callback suffix after its vector-facing turn helper returns. The original
  // thiscall frame is recreated so [ESP+0x88] remains the supplied influence argument.
  w(STACK,0);w(STACK+4,0);w(STACK+0x74,0);w(STACK+0x78,0);w(STACK+0x7c,-1);
  w(STACK+0x80,(int)RETURN);w(STACK+0x84,0x20004000);w(STACK+0x88,Float.floatToRawIntBits(influence));
  emu.writeRegister("ESI",PLAYER);emu.writeRegister("ESP",STACK);
  painCalls=statCalls=rngCalls=stringCalls=0;w(0x00502aacL,0x20008000);w(0x200086e4L,0x20009000);seed(seed);
  w(STACK+0x80,(int)RETURN);w(STACK+0x84,0x20004000);w(STACK+0x88,Float.floatToRawIntBits(influence));
  emu.writeRegister("ESI",PLAYER);emu.writeRegister("ESP",STACK);emu.getEmulator().setExecuteAddress(0x00451742L);
 }
 private String sample(String name,int now,int deadline,float dt,float influence,float exposure,int seed)throws Exception{
  init(now,deadline,dt,influence,exposure,seed);
  try{
   StringBuilder trace=new StringBuilder();
   for(int i=0;i<20000;i++){
    long pc=emu.getExecutionAddress().getOffset();
    if(pc==0x0045184cL||pc==0x00451855L){
     return "{\"name\":\""+name+"\",\"now\":"+Integer.toUnsignedLong(now)+",\"deadline_before\":"+Integer.toUnsignedLong(deadline)+",\"deadline_after\":"+Integer.toUnsignedLong(r(PLAYER+0x390))+",\"exposure_before\":"+exposure+",\"influence\":"+influence+",\"dt\":"+dt+",\"exposure_bits_after\":"+Integer.toUnsignedLong(r(PLAYER+0x3538))+",\"pain_stub_calls\":"+painCalls+",\"stat_stub_calls\":"+statCalls+",\"original_rng_helper_calls\":"+rngCalls+",\"string_stub_calls\":"+stringCalls+",\"passout_pending\":"+(pc==0x0045184cL)+",\"stop_pc\":\""+String.format("%08x",pc)+"\"}";
    }
    if(pc==RNG)rngCalls++;
    if(pc==STRING){stringCalls++;w(emu.readRegister("ECX").longValue(),0x00501908);ret(4,0);continue;}
    if(pc==PAIN){painCalls++;ret(8,1);continue;}
    if(pc==STAT){statCalls++;ret(8,1);continue;}
    if(pc==0x00450fb0L)throw new IllegalStateException("Passout reached; fixture must stop before the service call");
    if(trace.length()>1200)trace.delete(0,trace.length()-800);trace.append(String.format("%08x ",pc));
    if(!emu.step(monitor))throw new IllegalStateException("P-code failure at "+emu.getExecutionAddress()+": "+emu.getLastError()+"; trace="+trace);
   }
   throw new IllegalStateException("Suffix step limit at "+emu.getExecutionAddress());
  }finally{emu.dispose();}
 }
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: PlayerPainDeadlinePcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String>rows=new ArrayList<>();rows.add(sample("equal_deadline",1000,1000,0.0f,0.5f,0.25f,1234));rows.add(sample("due_no_passout",1001,1000,0.0f,0.5f,0.25f,1234));rows.add(sample("due_passout_pending",1001,1000,1.0f,1.0f,0.25f,1234));
  // Binary32 stores exactly 1.0, while original x87 ST(0) remains above 1.0.
  rows.add(sample("tiny_above_rounds_one",1001,1000,Math.scalb(1.0f,-55),1.0f,1.0f,1234));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"rng_seed\":1234,\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00451742 (suffix of 00451640 after vector-facing turn helper)\",\"scope\":\"Original unsigned deadline gate, seeded original 00498B30/00404AC0 draw, original 004B9400 conversion, deadline store, exposure arithmetic/strict passout comparison including a sub-ULP-above-one x87 case; 00402790 string construction, 004897C0 RC_Pain audio lookup/play and 00486980 stat service are controlled stubs; stop before 00450FB0 passout body\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original Player pain/deadline suffix cases");
 }
}
