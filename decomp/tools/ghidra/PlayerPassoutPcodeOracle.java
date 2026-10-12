import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute the original Player passout dispatcher with explicit service stubs. */
public class PlayerPassoutPcodeOracle extends GhidraScript {
 private static final long PLAYER=0x20000000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long LIST=0x20008000L,ACTOR=0x20009000L,VTABLE=0x2000a000L,CALLBACK=0x70000020L;
 private static final long ANIMATION=0x004a11a0L,STRING=0x00401d80L,CHATTER=0x004897c0L;
 private static final long LIST_CHECK=0x0048d490L,HERODOWN=0x0044ff10L;
 private EmulatorHelper emu;
 private int animationCalls,chatterCalls,listChecks,heroDownCalls,activeCalls,stringCalls;
 private long animationReceiver,activeReceiver;
 private int chatterFlag,heroDownArgument;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void ret(int args,int eax)throws Exception{
  long sp=emu.readRegister("ESP").longValue();long target=Integer.toUnsignedLong(r(sp));
  emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(target);
 }
 private void enter(int parameter,int activeIndex,int now,int state,int failedAt)throws Exception{
  emu=new EmulatorHelper(currentProgram);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(STACK),new byte[0x100]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
  emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  w(STACK,(int)RETURN);w(STACK+4,parameter);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PLAYER);
  w(0x00501918L,0);w(0x00501908L,0);w(0x0050190cL,0);w(0x00501910L,0);w(0x00501914L,0);
  w(0x00502afcL,now);w(PLAYER+0x18c,state);w(PLAYER+0x342c,failedAt);w(PLAYER+0x357c,activeIndex);
  w(PLAYER+0x3564,(int)LIST);w(LIST+activeIndex*4,0);
  if(activeIndex!=0){w(LIST+activeIndex*4,(int)ACTOR);w(ACTOR,(int)VTABLE);w(VTABLE+0x1c,(int)CALLBACK);}
  animationCalls=chatterCalls=listChecks=heroDownCalls=activeCalls=stringCalls=0;
  animationReceiver=activeReceiver=0;chatterFlag=heroDownArgument=0;
  emu.writeRegister("ESI",0);emu.writeRegister("EDI",0);emu.getEmulator().setExecuteAddress(0x00450fb0L);
 }
 private String sample(String name,int parameter,int activeIndex,int now,int state,int failedAt)throws Exception{
  enter(parameter,activeIndex,now,state,failedAt);
  try{
   for(int steps=0;steps<20000&&emu.getExecutionAddress().getOffset()!=RETURN;steps++){
    long pc=emu.getExecutionAddress().getOffset();
    if(pc==ANIMATION){animationCalls++;animationReceiver=emu.readRegister("ECX").longValue();ret(4,0);continue;}
    if(pc==STRING){stringCalls++;ret(4,0);continue;}
    if(pc==CHATTER){chatterCalls++;long sp=emu.readRegister("ESP").longValue();chatterFlag=r(sp+8);ret(8,0);continue;}
    if(pc==LIST_CHECK){listChecks++;ret(4,0);continue;}
    if(pc==HERODOWN){heroDownCalls++;long sp=emu.readRegister("ESP").longValue();heroDownArgument=r(sp+4);ret(4,0);continue;}
    if(pc==CALLBACK){activeCalls++;activeReceiver=emu.readRegister("ECX").longValue();long sp=emu.readRegister("ESP").longValue();long target=Integer.toUnsignedLong(r(sp));emu.writeRegister("ESP",sp+4);emu.getEmulator().setExecuteAddress(target);continue;}
    if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Passout instruction limit at "+emu.getExecutionAddress());
   long animationSubobjectOffset=animationCalls==0?0:animationReceiver-PLAYER;
   boolean delegated=parameter==2;
   return "{\"name\":\""+name+"\",\"parameter\":"+parameter+",\"active_object_index\":"+activeIndex+",\"clock\":"+Integer.toUnsignedString(now)+",\"failed_at_before\":"+Integer.toUnsignedString(failedAt)+",\"failed_at_after\":"+Integer.toUnsignedString(r(PLAYER+0x342c))+",\"state_before\":"+state+",\"state_after\":"+r(PLAYER+0x18c)+",\"motion_subobject_offset\":"+animationSubobjectOffset+",\"motion_called\":"+(animationCalls!=0)+",\"chatter_calls\":"+chatterCalls+",\"chatter_flag\":"+chatterFlag+",\"herodown_calls\":"+heroDownCalls+",\"herodown_argument\":"+heroDownArgument+",\"active_callback_calls\":"+activeCalls+",\"active_callback_index\":"+(activeCalls==0?0:activeIndex)+",\"active_callback_receiver\":"+Long.toUnsignedString(activeReceiver)+",\"list_helper_calls\":"+listChecks+",\"string_constructor_calls\":"+stringCalls+",\"delegated_to_herodown\":"+delegated+"}";
  }finally{emu.dispose();}
 }
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: PlayerPassoutPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<String>rows=new ArrayList<>();rows.add(sample("parameter_zero_no_active_object",0,0,123456,3,7));rows.add(sample("parameter_one_no_active_object",1,0,123457,4,8));rows.add(sample("parameter_one_active_object",1,1,123458,5,9));rows.add(sample("parameter_two_herodown",2,1,123459,5,10));rows.add(sample("other_parameter_failure_state",3,0,123460,2,11));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00450FB0\",\"scope\":\"Original passout animation selection, RC_PassOut shuffled-key call, parameter-2 early 0044FF10 delegation, clock/state stores, and optional active-object list/vtable +0x1C callback; animation, string/chatter, list-bounds, hero-down, and active-object method bodies are controlled stubs\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original Player passout cases");
 }
}
