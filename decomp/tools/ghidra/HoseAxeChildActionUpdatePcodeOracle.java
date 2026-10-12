import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute one Axe child vtable +0x18 callback (00460920), with service boundaries. */
public class HoseAxeChildActionUpdatePcodeOracle extends GhidraScript {
 private static final long CHILD=0x20010000L,MANAGER=0x20020000L,SCORE=0x20030000L,TEMP=0x20040000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long ENTRY=0x00460920L,STRING_CTOR=0x00401d80L,STRING_RELEASE=0x004011c0L,STAT=0x00486950L,HIDE=0x004a11a0L,RECOVERY=0x0045fa60L;
 private static final long ONCE=0x00501918L,STAT_RECORD=0x00501908L,SHARED_FLAG=0x00502e04L,CLOCK=0x00502afcL,MANAGER_GLOBAL=0x00502aacL;
 private EmulatorHelper emu;private final List<String> events=new ArrayList<>();
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private void w8(long p,int v)throws Exception{emu.writeMemory(a(p),new byte[]{(byte)v});}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private int b(long p)throws Exception{return emu.readMemory(a(p),1)[0]&255;}
 private void ret(int args,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();int target=r(sp);emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(target));}
 private String cstr(long p)throws Exception{StringBuilder s=new StringBuilder();for(int i=0;i<128;i++){int c=emu.readMemory(a(p+i),1)[0]&255;if(c==0)break;s.append((char)c);}return s.toString();}
 private String statString(long slot)throws Exception{long data=Integer.toUnsignedLong(r(slot));return data==STAT_RECORD?"Axe Hits":cstr(data+0xc);}
 private void init(int state,int start,int span,int clock,int once,int shared,int[]record)throws Exception{
  emu=new EmulatorHelper(currentProgram);events.clear();emu.writeMemory(a(CHILD),new byte[0x500]);emu.writeMemory(a(MANAGER),new byte[0x800]);emu.writeMemory(a(SCORE),new byte[0x100]);emu.writeMemory(a(TEMP),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  w(CHILD+0x368,state);w(CHILD+0x36c,start);w(CHILD+0x370,span);w8(ONCE,once);for(int i=0;i<4;i++)w(STAT_RECORD+i*4,record[i]);w8(SHARED_FLAG,shared);w(CLOCK,clock);w(MANAGER_GLOBAL,(int)MANAGER);w(MANAGER+0x6e4,(int)SCORE);
  w(STACK,(int)RETURN);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",CHILD);emu.getEmulator().setExecuteAddress(ENTRY);
 }
 private String sample(String name,int state,int start,int span,int clock,int once,int shared,int ctorRefCount,boolean ctorAliasesRecord,int[]record)throws Exception{
  init(state,start,span,clock,once,shared,record);
  for(int steps=0;steps<20000;steps++){
   long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;
   if(pc==STRING_CTOR){long slot=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();int source=r(sp+4);String text=cstr(Integer.toUnsignedLong(source));events.add("string_construct:00401D80:"+text);if(ctorAliasesRecord){w(slot,(int)STAT_RECORD);}else{w(slot,(int)TEMP);w(TEMP,ctorRefCount);w(TEMP+4,text.length());byte[]bytes=(text+"\0").getBytes(StandardCharsets.ISO_8859_1);emu.writeMemory(a(TEMP+0xc),bytes);}ret(4,0);continue;}
   if(pc==STAT){long receiver=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue(),slot=Integer.toUnsignedLong(r(sp+4));events.add("stat:00486950:"+(receiver-(SCORE+0x30))+":"+statString(slot));ret(4,0);continue;}
   if(pc==STRING_RELEASE){long slot=emu.readRegister("ECX").longValue();events.add("string_release:004011C0:"+statString(slot));ret(0,0);continue;}
   if(pc==HIDE){long receiver=emu.readRegister("ECX").longValue(),sp=emu.readRegister("ESP").longValue();events.add("hide:004A11A0:"+(receiver-CHILD)+":"+Integer.toUnsignedString(r(sp+4)));ret(4,0);continue;}
   if(pc==RECOVERY){long receiver=emu.readRegister("ECX").longValue();int arg=r(emu.readRegister("ESP").longValue()+4);events.add("recovery:0045FA60:"+(receiver-CHILD)+":"+Integer.toUnsignedString(arg));ret(4,0);continue;}
   if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
  }
  if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("00460920 instruction limit at "+emu.getExecutionAddress());
  StringBuilder rec=new StringBuilder("[");for(int i=0;i<4;i++){if(i>0)rec.append(',');rec.append(Integer.toUnsignedString(r(STAT_RECORD+i*4)));}rec.append(']');
  StringBuilder ev=new StringBuilder("[");for(int i=0;i<events.size();i++){if(i>0)ev.append(',');ev.append('"').append(events.get(i)).append('"');}ev.append(']');
  String row="{\"name\":\""+name+"\",\"input_ctor_refcount\":"+ctorRefCount+",\"input_ctor_alias_stat_record\":"+(ctorAliasesRecord?1:0)+",\"input_state\":"+state+",\"input_start\":"+Integer.toUnsignedString(start)+",\"input_span\":"+Integer.toUnsignedString(span)+",\"input_clock\":"+Integer.toUnsignedString(clock)+",\"input_stat_once\":"+once+",\"input_shared_flag\":"+shared+",\"input_stat_record\":["+Integer.toUnsignedString(record[0])+","+Integer.toUnsignedString(record[1])+","+Integer.toUnsignedString(record[2])+","+Integer.toUnsignedString(record[3])+"],\"child_state\":"+Integer.toUnsignedString(r(CHILD+0x368))+",\"stat_once\":"+b(ONCE)+",\"stat_record\":"+rec+",\"shared_flag\":"+b(SHARED_FLAG)+",\"events\":"+ev+"}";
  emu.dispose();return row;
 }
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseAxeChildActionUpdatePcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  int[]sentinel={0xaaaaaaaa,0xbbbbbbbb,0xcccccccc,0xdddddddd},initialized={2,0,1,0};List<String>rows=new ArrayList<>();
  rows.add(sample("idle_initializes_once",0,0,0,500,0,1,1,false,sentinel));rows.add(sample("idle_shared_stat_initialized",0,0,0,500,1,0,1,false,initialized));rows.add(sample("idle_string_refcount_retained",0,0,0,500,1,1,2,false,initialized));rows.add(sample("idle_constructor_alias_record",0,0,0,500,1,1,1,true,initialized));rows.add(sample("state1_noop",1,10,20,100,1,1,1,false,initialized));rows.add(sample("state2_due",2,100,100,201,1,1,1,false,initialized));rows.add(sample("state2_equal_deadline_noop",2,100,100,200,1,0,1,false,initialized));rows.add(sample("state2_unsigned_wrap_due",2,0xfffffff0,0x30,0x100,1,1,1,false,initialized));rows.add(sample("state2_future_noop",2,0x20,0x30,0x40,1,0,1,false,initialized));rows.add(sample("state3_noop",3,0,0,500,1,1,1,false,initialized));rows.add(sample("other_state_noop",4,0,0,500,1,0,1,false,initialized));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00460920\",\"scope\":\"Original Axe child vtable +0x18 body executes. String construction 00401D80, Axe Hits stat service 00486950, string release 004011C0, child hide 004A11A0, and recovery 0045FA60 are intercepted boundaries. The controlled string-constructor boundary returns either a temporary or the global-record alias, so the body's alias/refcount release branches execute in P-code. Global stat-once initialization, child state stores, timer sum/comparison, and absence of DAT_00502E04 writes execute/are captured in original P-code. No strike geometry, sound, caller cadence, or live Training state is covered.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" bounded Axe child +0x18 cases");
 }
}
