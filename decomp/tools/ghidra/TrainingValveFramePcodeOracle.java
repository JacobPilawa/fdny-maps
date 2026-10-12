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

/** Reproduce Training PropModel valve frames through original x86 transform P-code. */
public class TrainingValveFramePcodeOracle extends GhidraScript {
    private static final long STACK=0x20fff000L, RETURN=0x70000000L;
    private static final long PROP_MODEL=0x21000000L, USER_DATA=0x21000300L;
    private static final long MODEL_FRAME=0x21000400L, PROP_FRAME=0x21000500L, OUTPUT=0x21000600L;
    private static final long RESOLVE=0x004977b0L;
    private EmulatorHelper emu;

    private static final class Sample {
        final String name; final float heading,pitch,x,y,z;
        Sample(String n,float h,float p,float x,float y,float z){name=n;heading=h;pitch=p;this.x=x;this.y=y;this.z=z;}
    }

    private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
    private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
    private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
    private int f(float v){return Float.floatToRawIntBits(v);}

    private double x87Double(BigInteger value){
        BigInteger sig=value.and(BigInteger.ONE.shiftLeft(64).subtract(BigInteger.ONE));
        int signExp=value.shiftRight(64).intValue()&0xffff,exponent=signExp&0x7fff;
        if(exponent==0)return (signExp&0x8000)!=0?-0.0:0.0;
        double significand=new java.math.BigDecimal(sig).doubleValue()/0x1.0p63;
        double result=Math.scalb(significand,exponent-16383);
        return (signExp&0x8000)!=0?-result:result;
    }
    private BigInteger x87Value(double value){
        long bits=Double.doubleToRawLongBits(value);int exponent=(int)((bits>>>52)&0x7ff);
        long fraction=bits&0xfffffffffffffL;int sign=bits<0?0x8000:0;
        if(exponent==0)return BigInteger.valueOf(sign).shiftLeft(64);
        long significand=(fraction|0x10000000000000L)<<11;
        int signExp=sign|((exponent-1023+16383)&0x7fff);
        return BigInteger.valueOf(signExp).shiftLeft(64).or(new BigInteger(Long.toUnsignedString(significand)));
    }
    private boolean emulateTrig()throws Exception{
        long pc=emu.getExecutionAddress().getOffset(),next;boolean cosine;
        if(pc==0x004968c7L){next=0x004968c9L;cosine=true;}
        else if(pc==0x004968d0L){next=0x004968d2L;cosine=false;}
        else if(pc==0x00496b67L){next=0x00496b69L;cosine=true;}
        else if(pc==0x00496b77L){next=0x00496b79L;cosine=false;}
        else return false;
        double angle=x87Double(emu.readRegister("ST0"));
        emu.writeRegister("ST0",x87Value(cosine?Math.cos(angle):Math.sin(angle)));
        emu.getEmulator().setExecuteAddress(next);
        return true;
    }
    private void resetCpu()throws Exception{
        for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(n,0);
        emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
        Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)emu.writeRegister(cw,0x037fL);
        Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)emu.writeRegister(sw,0);
        Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)emu.writeRegister(tag,0xffffL);
        for(Register reg:currentProgram.getLanguage().getRegisters())
            if(reg.getName().toUpperCase().matches("ST[0-7]"))emu.writeRegister(reg,BigInteger.ZERO);
    }
    private void initFrame(long address,int[] position,int parent)throws Exception{
        emu.writeMemory(a(address),new byte[0x80]);w(address,0x004dfc30);
        int[] identity={0x3f800000,0,0,0,0x3f800000,0,0,0,0x3f800000};
        for(int i=0;i<9;i++)w(address+4+4L*i,identity[i]);
        for(int i=0;i<3;i++){w(address+0x28+4L*i,position[i]);w(address+0x34+4L*i,0x3f800000);}
        w(address+0x40,parent);
    }
    private void runTo(long stop,String label)throws Exception{
        for(int i=0;i<500000&&emu.getExecutionAddress().getOffset()!=stop;i++){
            if(emulateTrig())continue;
            if(!emu.step(monitor))throw new IllegalStateException(label+" P-code failed at "+emu.getExecutionAddress()+": "+emu.getLastError());
        }
        if(emu.getExecutionAddress().getOffset()!=stop)throw new IllegalStateException(label+" did not reach "+Long.toHexString(stop));
    }
    private String words(long address,int count)throws Exception{
        List<String>out=new ArrayList<>();for(int i=0;i<count;i++)out.add(Integer.toUnsignedString(r(address+4L*i)));
        return String.join(",",out);
    }
    private String sample(Sample s)throws Exception{
        emu=new EmulatorHelper(currentProgram);
        try{
            emu.writeMemory(a(PROP_MODEL),new byte[0x1c8]);emu.writeMemory(a(USER_DATA),new byte[0x80]);
            emu.writeMemory(a(MODEL_FRAME),new byte[0x80]);emu.writeMemory(a(PROP_FRAME),new byte[0x80]);
            emu.writeMemory(a(OUTPUT),new byte[0x80]);
            resetCpu();
            initFrame(MODEL_FRAME,new int[]{f(0),f(0),f(0)},(int)PROP_FRAME);
            initFrame(PROP_FRAME,new int[]{f(s.x),f(s.y),f(s.z)},0);

            // Execute the original 00443860 angle-conversion instruction slice
            // and its two real rotation calls. Only actor loading/registration
            // is skipped; the model frame and PropModel parent match ctor state.
            w(PROP_MODEL+0x1bc,(int)USER_DATA);w(PROP_MODEL+0xd0,(int)MODEL_FRAME);
            w(USER_DATA+0x18,f(s.heading));w(USER_DATA+0x1c,f(s.pitch));
            emu.writeRegister("ESI",PROP_MODEL);emu.writeRegister("EAX",USER_DATA);
            emu.writeRegister("ESP",STACK);emu.getEmulator().setExecuteAddress(0x00443948L);
            runTo(0x00443989L,"heading rotation");
            String localAfterHeading=words(MODEL_FRAME+4,9);
            runTo(0x0044399eL,"pitch rotation");
            String localAfterPitch=words(MODEL_FRAME+4,9);
            int headingAngle=r(USER_DATA+0x18),pitchAngle=r(USER_DATA+0x1c);

            resetCpu();
            emu.writeMemory(a(STACK),new byte[0x100]);w(STACK,(int)RETURN);w(STACK+4,(int)OUTPUT);w(STACK+8,(int)MODEL_FRAME);
            emu.writeRegister("ESP",STACK);emu.getEmulator().setExecuteAddress(RESOLVE);
            for(int i=0;i<100000&&emu.getExecutionAddress().getOffset()!=RETURN;i++){
                if(emulateTrig())continue;
                if(!emu.step(monitor))throw new IllegalStateException("Frame resolver failed at "+emu.getExecutionAddress()+": "+emu.getLastError());
            }
            if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Resolver limit at "+emu.getExecutionAddress());
            return "{\"name\":\""+s.name+"\",\"authored_heading_degrees\":"+s.heading+",\"authored_pitch_degrees\":"+s.pitch+
                ",\"angle_words\":["+Integer.toUnsignedString(headingAngle)+","+Integer.toUnsignedString(pitchAngle)+"]"+
                ",\"model_frame_axes_after_heading\":["+localAfterHeading+"]"+
                ",\"model_frame_axes_after_pitch\":["+localAfterPitch+"]"+
                ",\"resolved_axes\":["+words(OUTPUT+4,9)+"],\"resolved_position\":["+words(OUTPUT+0x28,3)+"]}";
        }finally{emu.dispose();}
    }

    @Override public void run()throws Exception{
        String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: TrainingValveFramePcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
        if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        long angleScale=currentProgram.getMemory().getLong(a(0x004e1120L));
        long circleUnits=currentProgram.getMemory().getLong(a(0x004e1118L));
        long headingOffset=currentProgram.getMemory().getLong(a(0x004e12d8L));
        int pitchScale=currentProgram.getMemory().getInt(a(0x004e12d4L));
        if(angleScale!=0x3fb5555555555555L||circleUnits!=0x4076800000000000L||
           headingOffset!=0x4056800000000000L||pitchScale!=0xc2b40000)
            throw new IllegalStateException("Unexpected original angle constants; refusing to record a fixture");
        Sample[]samples={new Sample("Training_Valve3_entity_159",9,0,1368,-176,-2080),new Sample("Training_Valve5_entity_164",3,0,416,-176,-2404)};
        List<String>rows=new ArrayList<>();for(Sample s:samples)rows.add(sample(s));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"original_instruction_slice\":\"00443948..0044399E\",\"original_entries\":[\"004968B0\",\"00496B50\",\"004977B0\"],\"angle_constants_bits\":[\"004E1120:0x3fb5555555555555\",\"004E1118:0x4076800000000000\",\"004E12D8:0x4056800000000000\",\"004E12D4:0xc2b40000\"],\"trig_limit\":\"Ghidra EmulatorHelper lacks FCOS/FSIN; these two x87 ops use Java Math double results converted back to x87 extended format\",\"scope\":\"The original 00443860 angle-conversion instructions and both rotation calls execute from 00443948 through 0044399E, followed by the original recursive frame resolver. Actor loading, callback scheduling, and live state are not executed/captured. The ACT root attachment and PropModel embedded frame are seeded from constructor/resource evidence.\",\"samples\":["+String.join(",",rows)+"]}\n");
        println("Captured "+rows.size()+" original Training valve frame transforms");
    }
}
