import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Executes BModel axe-hit leaf 0043E2A0; only script and destroy services stop. */
public class BModelAxeHitPcodeOracle extends GhidraScript {
 private static final long MODEL=0x20010000L,DATA=0x20020000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long ENTRY=0x0043e2a0L,STRING_CTOR=0x00402790L,SCRIPT_EVENT=0x004705b0L,DESTROY=0x0043d660L;
 private EmulatorHelper emu; private String currentCommand=""; private boolean chopResult,breakResult; private int scriptCalls,destroyCalls; private final List<String> events=new ArrayList<>();private final List<Long>constructedRefs=new ArrayList<>(),scriptReceivers=new ArrayList<>(),scriptRefs=new ArrayList<>(),destroyReceivers=new ArrayList<>();
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private void wf(long p,float v)throws Exception{w(p,Float.floatToRawIntBits(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private float f(long p)throws Exception{return Float.intBitsToFloat(r(p));}
 private String cstr(long p)throws Exception{StringBuilder s=new StringBuilder();for(int i=0;i<64;i++){int v=emu.readMemory(a(p+i),1)[0]&255;if(v==0)break;s.append((char)v);}return s.toString();}
 private void ret(int args,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();int target=r(sp);emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(target));}
 private void init(int hp,int scriptActor,float strikeTime,boolean chop,boolean breaking)throws Exception{
  emu=new EmulatorHelper(currentProgram);events.clear();constructedRefs.clear();scriptReceivers.clear();scriptRefs.clear();destroyReceivers.clear();currentCommand="";scriptCalls=0;destroyCalls=0;chopResult=chop;breakResult=breaking;
  emu.writeMemory(a(MODEL),new byte[0x300]);emu.writeMemory(a(DATA),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)emu.writeRegister(cw,0x037fL);Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)emu.writeRegister(sw,0);Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)emu.writeRegister(tag,0xffffL);
  w(MODEL+0xec,(int)DATA);w(DATA+0x34,hp);w(DATA+0x38,scriptActor);wf(MODEL+0x1ec,strikeTime);
  emu.writeMemory(a(0x00501908L),new byte[0x20]);w(0x00501908L,2);emu.writeMemory(a(0x00501918L),new byte[]{1});
  w(STACK,(int)RETURN);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",MODEL);emu.getEmulator().setExecuteAddress(ENTRY);
 }
 private String sample(String name,int hp,int scriptActor,float t,boolean chop,boolean breaking)throws Exception{
  init(hp,scriptActor,t,chop,breaking);
  for(int steps=0;steps<3000;steps++){
   long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;
   if(pc==0x0043e314L)events.add("hitpoints:decrement");
   if(pc==STRING_CTOR){long recv=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();long literal=Integer.toUnsignedLong(r(sp+4));currentCommand=cstr(literal);w(recv,0x00501908);constructedRefs.add(0x00501908L);events.add("string:00402790:"+currentCommand);ret(4,recv);continue;}
   if(pc==SCRIPT_EVENT){long recv=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();int stringObject=r(sp+4);if(recv!=MODEL+0x24||stringObject!=0x00501908)throw new IllegalStateException("004705B0 receiver or string object mismatch");scriptReceivers.add(recv);scriptRefs.add(Integer.toUnsignedLong(stringObject));events.add("script:"+currentCommand);scriptCalls++;ret(4,currentCommand.equals("chop")?chopResult?1:0:breakResult?1:0);continue;}
   if(pc==DESTROY){long recv=emu.readRegister("ECX").longValue();if(recv!=MODEL)throw new IllegalStateException("0043D660 receiver mismatch");destroyReceivers.add(recv);events.add("destroy:0043D660");destroyCalls++;ret(0,1);continue;}
   if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
  }
  if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("0043E2A0 instruction limit at "+emu.getExecutionAddress());
  long result=Integer.toUnsignedLong(emu.readRegister("EAX").intValue());
  return "{\"name\":\""+name+"\",\"model_address\":"+Long.toUnsignedString(MODEL)+",\"input\":{\"hitpoints\":"+hp+",\"script_actor\":"+scriptActor+",\"strike_time_bits\":"+Integer.toUnsignedString(Float.floatToRawIntBits(t))+",\"chop_result\":"+(chop?1:0)+",\"break_result\":"+(breaking?1:0)+"},\"hitpoints_after\":"+r(DATA+0x34)+",\"result_flags\":"+result+",\"script_calls\":"+scriptCalls+",\"destroy_calls\":"+destroyCalls+",\"constructed_refs\":"+longsJson(constructedRefs)+",\"script_receivers\":"+longsJson(scriptReceivers)+",\"script_refs\":"+longsJson(scriptRefs)+",\"destroy_receivers\":"+longsJson(destroyReceivers)+",\"events\":"+eventsJson()+"}";
 }
 private String eventsJson(){return events.isEmpty()?"[]":"[\""+String.join("\",\"",events)+"\"]";}
 private String longsJson(List<Long> values){List<String> q=new ArrayList<>();for(long value:values)q.add(Long.toUnsignedString(value));return "["+String.join(",",q)+"]";}
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: BModelAxeHitPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  float low=(float)-1e-28,high=(float)1e-28;List<String>rows=new ArrayList<>();
  rows.add(sample("hp_zero_training_doors",0,0,high,false,false));rows.add(sample("script_actor_present",2,0x12345678,high,false,false));rows.add(sample("interior_strike_time",2,0,0.5f,false,false));rows.add(sample("lower_nearest_float_outside",2,0,low,true,false));rows.add(sample("lower_neighbor_inside",2,0,Math.nextUp(low),false,false));rows.add(sample("upper_nearest_float_outside_break_and_all_events",1,0,high,true,true));rows.add(sample("upper_neighbor_inside",2,0,Math.nextDown(high),false,false));rows.add(sample("upper_outside_next_float",2,0,Math.nextUp(high),false,false));rows.add(sample("lower_outside_next_float",2,0,Math.nextDown(low),false,false));rows.add(sample("upper_outside_break_event_false",1,0,high,true,false));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"0043E2A0\",\"time_field_width\":4,\"threshold_operand_width\":8,\"threshold_bits\":{\"lower_004E0B60\":\"0x"+Long.toHexString(Double.doubleToRawLongBits(-1e-28))+"\",\"upper_004E0B68\":\"0x"+Long.toHexString(Double.doubleToRawLongBits(1e-28))+"\"},\"scope\":\"Original BModel axe-hitpoint leaf executes. String construction 00402790, script command dispatch 004705B0, and destruction callback 0043D660 are explicit boundaries; fixture verifies receiver/command order and local hitpoint/result writes. The float time field is compared directly to binary64 thresholds, so neighboring representable float cases cover both edges; exact equality to either double is impossible. This is not full Axe strike geometry, candidate scheduling, or event interpreter behavior.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original BModel Axe-hit cases");
 }
}
