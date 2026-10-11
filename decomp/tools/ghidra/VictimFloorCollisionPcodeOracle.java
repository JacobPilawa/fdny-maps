import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Run Genesis.dll's hull collision path under Ghidra's software P-code emulator. */
public class VictimFloorCollisionPcodeOracle extends GhidraScript {
    private static final long STATE = 0x20000000L;
    private static final long STACK = 0x20fff000L;
    private static final long IO = 0x20f00000L;
    private static final long RETURN = 0x70000100L;
    private static final long ENTRY = 0x10001e30L;
    private static final long CONTACT_COMMIT = 0x100371b3L;
    private static final long LAST_LEAF = 0x10362f10L;
    private static final int MODEL_RECORD_SIZE = 212;
    private static final int MAX_INSTRUCTIONS = 100000;

    private Address address(long offset) {
        return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(offset);
    }

    private static float f32(float value) {
        return Float.intBitsToFloat(Float.floatToRawIntBits(value));
    }

    private static int readInt(byte[] bytes, int offset) {
        return ByteBuffer.wrap(bytes, offset, 4).order(ByteOrder.LITTLE_ENDIAN).getInt();
    }

    private static float readFloat(byte[] bytes, int offset) {
        return Float.intBitsToFloat(readInt(bytes, offset));
    }

    private static String bits(float value) {
        return String.format("%08x", Float.floatToRawIntBits(value));
    }

    private static String number(float value) {
        if (!Float.isFinite(value)) throw new IllegalStateException("non-finite output float");
        return Float.toString(value);
    }

    private static String vector(float[] values) {
        return "[" + number(values[0]) + "," + number(values[1]) + "," + number(values[2]) + "]";
    }

    private static String bitVector(float[] values) {
        return "[\"" + bits(values[0]) + "\",\"" + bits(values[1]) + "\",\"" +
            bits(values[2]) + "\"]";
    }

    private static void push(ByteBuffer stack, long value) {
        stack.putInt((int) value);
    }

    private String sha256(Path path) throws Exception {
        byte[] digest = MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(path));
        StringBuilder result = new StringBuilder();
        for (byte value : digest) result.append(String.format("%02x", value));
        return result.toString();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 5) {
            printerr("Usage: VictimFloorCollisionPcodeOracle.java <state.bin> <candidates.tsv> " +
                "<output.json> <Genesis.dll> <collision.json>");
            return;
        }
        if (!currentProgram.getLanguageID().toString().startsWith("x86:LE:32:")) {
            throw new IllegalStateException("Expected an x86 32-bit Genesis.dll Ghidra program");
        }
        Function entryFunction = getFunctionAt(address(ENTRY));
        if (entryFunction == null || !"geWorld_Collision".equals(entryFunction.getName())) {
            throw new IllegalStateException("Expected geWorld_Collision at 0x10001e30");
        }
        Instruction commit = getInstructionAt(address(CONTACT_COMMIT));
        if (commit == null || !"MOV".equals(commit.getMnemonicString()) ||
            !commit.toString().contains("10362f44") || !commit.toString().contains("0x1")) {
            throw new IllegalStateException("Hull contact marker 0x100371b3 no longer matches " +
                "the verified DAT_10362f44=1 instruction: " + commit);
        }

        Path statePath = Path.of(args[0]);
        Path candidatesPath = Path.of(args[1]);
        Path outputPath = Path.of(args[2]);
        byte[] state = Files.readAllBytes(statePath);
        if (state.length != 0x1000000) throw new IllegalArgumentException("Expected a 16 MiB state image");
        ByteBuffer stateView = ByteBuffer.wrap(state).order(ByteOrder.LITTLE_ENDIAN);
        long runtimeModels = Integer.toUnsignedLong(stateView.getInt(0x1000 + 464));
        List<String> candidates = Files.readAllLines(candidatesPath);
        List<String> samples = new ArrayList<>();

        for (String line : candidates) {
            if (line.isBlank()) continue;
            String[] fields = line.split("\\t");
            if (fields.length != 6) throw new IllegalArgumentException("Bad candidate row: " + line);
            String victimName = fields[0];
            int victimEntity = Integer.parseInt(fields[1]);
            int candidateEntity = Integer.parseInt(fields[2]);
            float x = f32(Float.parseFloat(fields[3]));
            float y = f32(Float.parseFloat(fields[4]));
            float z = f32(Float.parseFloat(fields[5]));
            float startY = f32(y + 48.0f);
            float endY = f32(startY - 1000.0f);

            EmulatorHelper emulator = new EmulatorHelper(currentProgram);
            try {
                emulator.writeMemory(address(STATE), state);
                ByteBuffer io = ByteBuffer.allocate(48).order(ByteOrder.LITTLE_ENDIAN);
                for (float value : new float[]{-24, -24, -24, 24, 24, 24,
                        x, startY, z, x, endY, z}) io.putFloat(value);
                emulator.writeMemory(address(IO), io.array());
                emulator.writeMemory(address(IO + 0x100), new byte[64]);

                ByteBuffer stack = ByteBuffer.allocate(48).order(ByteOrder.LITTLE_ENDIAN);
                push(stack, RETURN);
                push(stack, STATE);
                push(stack, IO);
                push(stack, IO + 12);
                push(stack, IO + 24);
                push(stack, IO + 36);
                push(stack, 0x43); // contents
                push(stack, 2); // BSP model collision flag
                push(stack, 0);
                push(stack, 0);
                push(stack, 0);
                push(stack, IO + 0x100); // output trace structure
                emulator.writeMemory(address(STACK), stack.array());

                emulator.writeRegister("ESP", STACK);
                emulator.writeRegister("EFLAGS", 0x202);
                emulator.writeRegister("DF", 0);
                for (String register : new String[]{"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP"}) {
                    emulator.writeRegister(register, 0);
                }
                emulator.writeRegister("FPUControlWord", 0x037f);
                emulator.writeRegister("FPUStatusWord", 0);
                emulator.writeRegister("FPUTagWord", 0xffff);
                for (String register : new String[]{"ST0", "ST1", "ST2", "ST3", "ST4", "ST5", "ST6", "ST7"}) {
                    emulator.writeRegister(register, BigInteger.ZERO);
                }
                emulator.setBreakpoint(address(RETURN));
                emulator.getEmulator().setExecuteAddress(ENTRY);

                int steps = 0;
                boolean contactCommitted = false;
                while (steps < MAX_INSTRUCTIONS && !emulator.getExecutionAddress().equals(address(RETURN))) {
                    if (emulator.getExecutionAddress().equals(address(CONTACT_COMMIT))) contactCommitted = true;
                    if (!emulator.step(monitor)) {
                        throw new IllegalStateException("P-code failure at " + emulator.getExecutionAddress() +
                            " after " + steps + " instructions: " + emulator.getLastError());
                    }
                    steps++;
                }
                if (!emulator.getExecutionAddress().equals(address(RETURN))) {
                    throw new IllegalStateException("Instruction limit reached at " + emulator.getExecutionAddress());
                }

                byte[] result = emulator.readMemory(address(IO + 0x100), 40);
                boolean hit = emulator.readRegister("EAX").intValue() != 0;
                int modelPointer = readInt(result, 0);
                long model = modelPointer == 0 ? -1 :
                    (Integer.toUnsignedLong(modelPointer) - runtimeModels) / MODEL_RECORD_SIZE;
                byte[] leafBytes = emulator.readMemory(address(LAST_LEAF), 4);
                int leaf = readInt(leafBytes, 0);
                float[] contact = {readFloat(result, 12), readFloat(result, 16), readFloat(result, 20)};
                float distance = readFloat(result, 24);
                float[] normal = {readFloat(result, 28), readFloat(result, 32), readFloat(result, 36)};
                if (!hit || !contactCommitted) {
                    throw new IllegalStateException("No committed hull contact for candidate " + candidateEntity +
                        " (return=" + hit + ", marker=" + contactCommitted + ")");
                }
                float[] finalPosition = {contact[0], f32(contact[1] + 1.0f), contact[2]};
                samples.add("    {\"victim\":\"" + victimName + "\",\"victim_entity\":" + victimEntity +
                    ",\"candidate_entity\":" + candidateEntity + ",\"authored_position\":" +
                    vector(new float[]{x, y, z}) + ",\"start\":" + vector(new float[]{x, startY, z}) +
                    ",\"delta\":[0,-1000,0],\"mins\":[-24,-24,-24],\"maxs\":[24,24,24]," +
                    "\"contents\":67,\"flags\":2,\"expected_hit\":true,\"recorded_contact\":true," +
                    "\"model\":" + model + ",\"leaf\":" + Integer.toUnsignedString(leaf) +
                    ",\"contact\":" + vector(contact) + ",\"contact_bits\":" + bitVector(contact) +
                    ",\"distance\":" + number(distance) + ",\"distance_bits\":\"" + bits(distance) +
                    "\",\"normal\":" + vector(normal) + ",\"normal_bits\":" + bitVector(normal) +
                    ",\"expected_position\":" + vector(finalPosition) +
                    ",\"expected_position_bits\":" + bitVector(finalPosition) +
                    ",\"emulated_instructions\":" + steps + "}");
                println("candidate=" + candidateEntity + " model=" + model + " leaf=" +
                    Integer.toUnsignedString(leaf) + " contact=" + vector(contact) +
                    " recorded=" + contactCommitted + " steps=" + steps);
            }
            finally {
                emulator.dispose();
            }
        }

        StringBuilder json = new StringBuilder();
        json.append("{\n  \"provenance\": {\n")
            .append("    \"state_sha256\": \"").append(sha256(statePath)).append("\",\n")
            .append("    \"candidates_sha256\": \"").append(sha256(candidatesPath)).append("\",\n")
            .append("    \"dll_sha256\": \"").append(sha256(Path.of(args[3]))).append("\",\n")
            .append("    \"bsp_sha256\": \"").append(sha256(Path.of(args[4]))).append("\",\n")
            .append("    \"entry\": \"Genesis.dll geWorld_Collision 0x10001E30; hull path 0x100372A0; leaf traversal 0x100371F0 -> 0x10036EA0\",\n")
            .append("    \"contact_commit_instruction\": \"0x100371B3: MOV dword ptr [0x10362F44],0x1, verified from Ghidra listing\",\n")
            .append("    \"method\": \"Ghidra EmulatorHelper software P-code emulation of original PE32 instructions; fresh state per sample; EFLAGS=0x202 and DF=0; FPU control/status/tag and x87 stack initialized to reset state\",\n")
            .append("    \"scope\": \"Six authored Training victim positions, (-24,-24,-24)/(24,24,24) hull, contents 0x43, flags 2; per the original floor helper start Y is raised by 48, sweep is 1000 downward, and final position adds 1 to contact Y\"\n")
            .append("  },\n  \"samples\": [\n");
        for (int i = 0; i < samples.size(); i++) {
            json.append(samples.get(i));
            json.append(i + 1 == samples.size() ? "\n" : ",\n");
        }
        json.append("  ]\n}\n");
        Files.createDirectories(outputPath.getParent());
        Files.writeString(outputPath, json.toString());
        println("Saved " + samples.size() + " Genesis.dll Training victim hull contacts to " + outputPath);
    }
}
