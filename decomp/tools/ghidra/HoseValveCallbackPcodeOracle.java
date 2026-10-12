import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute original PropModel valve callback 00444120 with owner/event boundaries recorded. */
public class HoseValveCallbackPcodeOracle extends GhidraScript {
 private static final long ROOT=0x20000000L,PROP=ROOT+0x150,PLAYER=0x20010000L,APP=0x20020000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long OWNER=0x0044c840L,STRING=0x00401d80L,DISPATCH=0x004705b0L;
 private EmulatorHelper emu;private int ownerCalls,stringCalls,dispatchCalls,ownerAction;
 private long ownerReceiver,ownerValve,dispatchReceiver;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[] b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void ret(int argumentBytes,int eax)throws Exception{long sp=emu.readRegister("ESP").longValue();long target=Integer.toUnsignedLong(r(sp));emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+argumentBytes);emu.getEmulator().setExecuteAddress(target);}
 private String sample(String name,int state,int action)throws Exception{
  emu=new EmulatorHelper(currentProgram);
  try{
   emu.writeMemory(a(ROOT),new byte[0x400]);emu.writeMemory(a(PLAYER),new byte[0x100]);emu.writeMemory(a(STACK),new byte[0x200]);
   for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
   emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
   w(0x00503a78L,(int)APP);w(APP+0x30,(int)PLAYER);w(PROP+0x70,state);
   w(0x00501918L,0);w(0x00501908L,0);w(0x0050190cL,0);w(0x00501910L,0);w(0x00501914L,0);
   w(STACK,(int)RETURN);w(STACK+4,action);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PROP);emu.getEmulator().setExecuteAddress(0x00444120L);
   ownerCalls=stringCalls=dispatchCalls=ownerAction=0;ownerReceiver=ownerValve=dispatchReceiver=0;
   for(int steps=0;steps<10000&&emu.getExecutionAddress().getOffset()!=RETURN;steps++){
    long pc=emu.getExecutionAddress().getOffset();
    if(pc==OWNER){ownerCalls++;ownerReceiver=emu.readRegister("ECX").longValue();long sp=emu.readRegister("ESP").longValue();ownerValve=Integer.toUnsignedLong(r(sp+4));ownerAction=r(sp+8);ret(8,0);continue;}
    if(pc==STRING){stringCalls++;ret(4,0);continue;}
    if(pc==DISPATCH){dispatchCalls++;dispatchReceiver=emu.readRegister("ECX").longValue();ret(4,0);continue;}
    if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Valve callback instruction limit at "+emu.getExecutionAddress());
   return "{\"name\":\""+name+"\",\"prop_model_state\":"+state+",\"action\":"+action+
    ",\"owner_action_calls\":"+ownerCalls+",\"owner_receiver_offset\":"+(ownerCalls==0?0:ownerReceiver-PLAYER)+
    ",\"valve_argument_root_offset\":"+(ownerCalls==0?0:ownerValve-ROOT)+",\"owner_action\":"+ownerAction+
    ",\"string_constructor_calls\":"+stringCalls+",\"script_use_calls\":"+dispatchCalls+
    ",\"script_receiver_root_offset\":"+(dispatchCalls==0?0:dispatchReceiver-ROOT)+"}";
  }finally{emu.dispose();}
 }
 @Override public void run()throws Exception{
  String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseValveCallbackPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String> rows=new ArrayList<>();rows.add(sample("state0_attach_code",0,1001));rows.add(sample("state0_detach_code",0,1002));rows.add(sample("valve_attach",1,1001));rows.add(sample("valve_detach",1,1002));rows.add(sample("valve_other_action",1,7));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00444120\",\"scope\":\"Original PropModel callback branch, owner-action receiver/valve-root/action arguments, string-construction and script-event forwarding; 0044C840, 00401D80, and 004705B0 are intercepted service boundaries, so owner attachment side effects and script interpreter effects are excluded\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original hose-valve callback cases");
 }
}
