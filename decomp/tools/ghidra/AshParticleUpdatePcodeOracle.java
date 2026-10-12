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

/** Execute original 00446510 against controlled particle fields and the real shared RNG. */
public class AshParticleUpdatePcodeOracle extends GhidraScript {
    private static final long PARTICLE=0x20000000L, STACK=0x20fff000L, RETURN=0x70000000L;
    private static final long SEED=0x00404b50L, UPDATE=0x00446510L, RANGE=0x00498b30L;
    private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
    private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
    private int r(EmulatorHelper e,long p)throws Exception{
        byte[] b=e.readMemory(a(p),4);
        return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);
    }
    private int byteAt(EmulatorHelper e,long p)throws Exception{return e.readMemory(a(p),1)[0]&255;}
    private void zeroCpu(EmulatorHelper e)throws Exception{
        for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
        e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
        Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
        Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
        Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
        for(Register reg:currentProgram.getLanguage().getRegisters())
            if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,BigInteger.ZERO);
    }
    private void runToReturn(EmulatorHelper e,long entry,int maxSteps,String label)throws Exception{
        e.getEmulator().setExecuteAddress(entry);
        for(int i=0;i<maxSteps&&e.getExecutionAddress().getOffset()!=RETURN;i++)
            if(!e.step(monitor))throw new IllegalStateException(label+" P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
        if(e.getExecutionAddress().getOffset()!=RETURN)
            throw new IllegalStateException(label+" instruction limit at "+e.getExecutionAddress());
    }
    private String sample(String name,int seed,int now,boolean active,int deadline,
                          int xBits,int yBits,int zBits,int secondYBits)throws Exception{
        EmulatorHelper e=new EmulatorHelper(currentProgram);
        try{
            e.writeMemory(a(PARTICLE),new byte[0x200]);e.writeMemory(a(STACK),new byte[0x100]);zeroCpu(e);
            w(e,PARTICLE+0x30,active?1:0);w(e,PARTICLE+0x168,deadline);
            w(e,PARTICLE+0x7c,xBits);w(e,PARTICLE+0x80,yBits);w(e,PARTICLE+0x84,zBits);
            w(e,PARTICLE+0x8c,secondYBits);w(e,0x00502afcL,now);
            w(e,STACK,(int)RETURN);w(e,STACK+4,seed);e.writeRegister("ESP",STACK);e.writeRegister("ECX",PARTICLE);
            runToReturn(e,SEED,20000,"RNG seed");

            zeroCpu(e);w(e,0x00502afcL,now);w(e,STACK,(int)RETURN);
            e.writeRegister("ESP",STACK);e.writeRegister("ECX",PARTICLE);e.getEmulator().setExecuteAddress(UPDATE);
            int rangeCalls=0;
            for(int steps=0;steps<20000&&e.getExecutionAddress().getOffset()!=RETURN;steps++){
                if(e.getExecutionAddress().getOffset()==RANGE)rangeCalls++;
                if(!e.step(monitor))throw new IllegalStateException("Update P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
            }
            if(e.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Update instruction limit at "+e.getExecutionAddress());
            return "{\"name\":\""+name+"\",\"seed\":"+Integer.toUnsignedString(seed)+
                ",\"now\":"+Integer.toUnsignedString(now)+",\"active_before\":"+active+
                ",\"deadline_before\":"+Integer.toUnsignedString(deadline)+
                ",\"vector7c_before\":["+Integer.toUnsignedLong(xBits)+","+Integer.toUnsignedLong(yBits)+","+Integer.toUnsignedLong(zBits)+"]"+
                ",\"vector88_y_before\":"+Integer.toUnsignedLong(secondYBits)+
                ",\"range_calls\":"+rangeCalls+
                ",\"active_after\":"+(byteAt(e,PARTICLE+0x30)!=0)+
                ",\"deadline_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x168))+
                ",\"vector_dirty_after\":"+(byteAt(e,PARTICLE+0xa0)!=0)+
                ",\"vector7c_after\":["+Integer.toUnsignedLong(r(e,PARTICLE+0x7c))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x80))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x84))+"]"+
                ",\"vector88_y_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x8c))+"}";
        }finally{e.dispose();}
    }
    @Override public void run()throws Exception{
        String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshParticleUpdatePcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
        if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        int y0=Float.floatToRawIntBits(0.0f),y25=Float.floatToRawIntBits(2.5f),yn4=Float.floatToRawIntBits(-4.0f);
        List<String> rows=new ArrayList<>();
        rows.add(sample("due_zero_vectors",0x1234,100,true,100,0,y0,0,0));
        rows.add(sample("due_nonzero_prior_y",0x13579bdf,0x10203040,true,0x10203040,Float.floatToRawIntBits(3.25f),y25,Float.floatToRawIntBits(-7.0f),yn4));
        rows.add(sample("active_not_due",0x1234,199,true,200,0,y25,0,yn4));
        rows.add(sample("unsigned_deadline_future_across_wrap",0x2468,0x10,true,0xfffffff0,0,y25,0,yn4));
        rows.add(sample("inactive",0x1234,199,false,0,0,y25,0,yn4));
        rows.add(sample("unsigned_clock_wrap",0xabcdef01,0xffffff00,true,0,0,y25,0,yn4));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00446510\",\"rng_seed_entry\":\"00404B50\",\"range_entry\":\"00498B30\",\"scope\":\"Due/inactive/deadline update branch and original seeded range calls; 0047F1B0, 0047F1C0, and 0042D280 execute as original code; presentation and frame scheduling are outside scope\",\"samples\":["+String.join(",",rows)+"]}\n");
        println("Captured "+rows.size()+" original ash particle update cases");
    }
}
