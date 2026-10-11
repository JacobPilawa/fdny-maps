/** Explicit subset of the shipped EVS syntax. Unsupported commands are reported.
 * Scheduling uses simulation time; this is not yet a port of the retail VM.
 */
export interface ScriptEntity { index: number; properties: Record<string, string> }
export interface ScriptHost {
  hint(text: string, seconds: number): void;
  player(command: string): boolean;
  entity?(target: string, command: string): boolean;
  environment?(command: string): boolean;
  clear?(zone: string): boolean;
  unsupported(command: string, source: string): void;
}

function unwrap(text: string): string {
  text = text.trim();
  while ((text.startsWith("(") && text.endsWith(")")) || (text.startsWith("[") && text.endsWith("]"))) {
    const opening = text[0], closing = text[text.length - 1];
    let depth = 0, encloses = true;
    for (let i = 0; i < text.length - 1; i++) {
      if (text[i] === opening) depth++;
      if (text[i] === closing) depth--;
      if (depth === 0) { encloses = false; break; }
    }
    if (!encloses) break;
    text = text.slice(1, -1).trim();
  }
  return text;
}

function splitCommands(text: string): string[] {
  const result: string[] = [];
  let depth = 0, start = 0;
  for (let i = 0; i < text.length; i++) {
    if (text[i] === "(" || text[i] === "[") depth++;
    if (text[i] === ")" || text[i] === "]") depth--;
    if (text[i] === "," && depth === 0) { result.push(text.slice(start, i)); start = i + 1; }
  }
  result.push(text.slice(start));
  return result;
}

export class TrainingScripts {
  readonly variables = new Map<string, string>();
  private readonly once = new Set<string>();
  private readonly reported = new Set<string>();
  private pending: { at: number; command: string; source: string }[] = [];
  private time = 0;
  private readonly host: ScriptHost;
  private readonly definitions: { text: string; source: string }[] = [];
  private loadingDefinitions = false;

  constructor(host: ScriptHost) { this.host = host; }

  loadDefinitions(text: string, source: string): void {
    if (!this.loadingDefinitions) this.definitions.push({ text, source });
    this.loadingDefinitions = true;
    text.split(/\r?\n/).forEach((line, index) => {
      // Only definitions are loaded here. exec/global event registration and
      // non-definition startup commands remain outside this subset.
      if (/^\s*set\s/.test(line)) this.run(line, `${source}:${index + 1}`);
    });
    this.loadingDefinitions = false;
  }

  /** Level restart semantics: clear runtime state and replay startup sets. */
  reset(atSeconds = 0): void {
    this.variables.clear();
    this.once.clear();
    this.reported.clear();
    this.pending = [];
    this.time = atSeconds;
    const definitions = this.definitions.slice();
    this.loadingDefinitions = true;
    for (const definition of definitions) {
      definition.text.split(/\r?\n/).forEach((line, index) => {
        if (/^\s*set\s/.test(line)) this.run(line, `${definition.source}:${index + 1}`);
      });
    }
    this.loadingDefinitions = false;
  }

  dispatch(entity: ScriptEntity, event: string): void {
    for (const [key, value] of Object.entries(entity.properties).sort(([a], [b]) => {
      const ai = Number(a.match(/\d+$/)?.[0] ?? 0), bi = Number(b.match(/\d+$/)?.[0] ?? 0);
      return ai - bi;
    })) {
      if (!/^S_Script\d+$/.test(key)) continue;
      const match = value.match(/^\s*([a-z]+):\s*(.*)$/i);
      if (match?.[1] === event) this.run(match[2], `${entity.index}:${key}`);
    }
  }

  advance(dt: number): void {
    this.time += dt;
    const due = this.pending.filter((item) => item.at <= this.time);
    this.pending = this.pending.filter((item) => item.at > this.time);
    due.sort((a, b) => a.at - b.at).forEach((item) => this.run(item.command, item.source));
  }

  run(raw: string, source: string, depth = 0): void {
    const command = unwrap(raw);
    if (!command || command === "<null>") return;
    if (depth > 32) { this.report(command, source); return; }
    let match: RegExpMatchArray | null;
    // Macros are frequently defined as event wrappers (for example
    // `set Quake (touch: once (...))`) and then invoked by another trigger.
    // Calling the macro directly executes the wrapped body in the retail VM.
    if ((match = command.match(/^(?:touch|touching|untouch|use|pickup|break|chop):\s*([\s\S]+)$/i))) {
      this.run(match[1], `${source}/event`, depth + 1); return;
    }
    if ((match = command.match(/^set\s+(\w+)\s+([\s\S]+)$/))) {
      this.variables.set(match[1], unwrap(match[2])); return;
    }
    // A small number of shipped mission scripts use the VM's integer counter
    // verbs for one-shot/revisit gates (Downtown Bar's club sequence is the
    // authored example). Missing variables read as zero in the retail VM.
    if ((match = command.match(/^(inc|dec)\s+(\w+)$/i))) {
      const current = Number(this.variables.get(match[2]) ?? "0");
      const value = Number.isFinite(current) ? current : 0;
      this.variables.set(match[2], String(value + (match[1].toLowerCase() === "inc" ? 1 : -1)));
      return;
    }
    if ((match = command.match(/^\*(\w+)$/))) {
      const value = this.variables.get(match[1]);
      if (value !== undefined) this.run(value, `${source}/*${match[1]}`, depth + 1);
      else this.report(command, source);
      return;
    }
    if ((match = command.match(/^(if|ifnot)\s+(\w+)\s+(\S+)\s+(\([\s\S]*\))$/))) {
      const equal = this.variables.get(match[2]) === match[3];
      if (equal === (match[1] === "if")) this.run(match[4], `${source}/condition`, depth + 1);
      return;
    }
    if ((match = command.match(/^ifclear\s+(\w+)\s+(\([\s\S]*\))$/i))) {
      if (this.host.clear?.(match[1]) ?? false) this.run(match[2], `${source}/clear`, depth + 1);
      return;
    }
    if ((match = command.match(/^ifnotclear\s+(\w+)\s+(\([\s\S]*\))$/i))) {
      if (!(this.host.clear?.(match[1]) ?? false)) this.run(match[2], `${source}/notclear`, depth + 1);
      return;
    }
    if ((match = command.match(/^once\s+(\([\s\S]*\))$/))) {
      if (!this.once.has(source)) { this.once.add(source); this.run(match[1], `${source}/once`, depth + 1); }
      return;
    }
    if ((match = command.match(/^(que|cmd)\s*\[([\s\S]*)\]$/))) {
      let delay = 0;
      splitCommands(match[2]).forEach((part, index) => {
        const wait = unwrap(part).match(/^wait\s+(\d+(?:\.\d+)?)$/);
        if (match![1] === "que" && wait) { delay += Number(wait[1]); return; }
        const origin = `${source}/${index}`;
        if (delay > 0) this.pending.push({ at: this.time + delay, command: part, source: origin });
        else this.run(part, origin, depth + 1);
      });
      return;
    }
    if ((match = command.match(/^addtimecmd\s+(\d+(?:\.\d+)?)\s+([\s\S]+)$/i))) {
      this.pending.push({ at: this.time + Number(match[1]), command: match[2], source: `${source}/addtimecmd` });
      return;
    }
    if ((match = command.match(/^hint\s+(\d+(?:\.\d+)?)\s+\(([\s\S]*)\)$/))) {
      this.host.hint(match[2], Number(match[1])); return;
    }
    if (/^(?:fogenable|fogdisable)\b/i.test(command)) {
      const expanded = command.replace(/\*(\w+)/g, (_, name: string) => this.variables.get(name) ?? `*${name}`);
      if (!(this.host.environment?.(expanded) ?? false)) this.report(command, source);
      return;
    }
    if ((match = command.match(/^(?:tell|action)\s+player\s+([\s\S]+)$/))) {
      if (!this.host.player(unwrap(match[1]))) this.report(command, source);
      return;
    }
    // Mission maps use `tell *GroupName command` for a variable containing a
    // bracketed target list (Subway B's two fire-door valves). Expand the
    // group exactly like the literal `tell [a,b] command` form.
    if ((match = command.match(/^(?:tell|action)\s+\*(\w+)\s+([\s\S]+)$/i))) {
      const value = this.variables.get(match[1]);
      const list = value && unwrap(value);
      const targetText = list?.startsWith("[") && list.endsWith("]") ? list.slice(1, -1) : list;
      const targets = targetText ? splitCommands(targetText).map((target) => target.trim()).filter(Boolean) : [];
      const action = unwrap(match[2]);
      if (!targets.length || targets.some((target) => !this.host.entity?.(target, action))) this.report(command, source);
      return;
    }
    if ((match = command.match(/^(?:tell|action)\s+\[([^\]]+)\]\s+([\s\S]+)$/))) {
      const targets = match[1].split(",").map((target) => target.trim()).filter(Boolean);
      const action = unwrap(match[2]);
      if (!targets.length || targets.some((target) => !this.host.entity?.(target, action))) this.report(command, source);
      return;
    }
    if ((match = command.match(/^(?:tell|action)\s+([A-Za-z0-9_]+)\s+([\s\S]+)$/))) {
      const action = unwrap(match[2]);
      const queued = action.match(/^que\s*\[([\s\S]*)\]$/i);
      if (queued) {
        let delay = 0, emitted = 0;
        for (const part of splitCommands(queued[1])) {
          const wait = unwrap(part).match(/^wait\s+(\d+(?:\.\d+)?)$/i);
          if (wait) { delay += Number(wait[1]); continue; }
          const nested = `tell ${match[1]} ${unwrap(part)}`;
          if (delay > 0) this.pending.push({ at: this.time + delay, command: nested, source: `${source}/queue${emitted++}` });
          else if (!this.host.entity?.(match[1], unwrap(part))) this.report(command, source);
        }
      } else if (!this.host.entity?.(match[1], action)) this.report(command, source);
      return;
    }
    // Training uses these only for presentation (extinguisher model scale and
    // a Steam4 cue). Consume them so they cannot introduce script warnings or
    // alter gameplay state in the mechanics-focused VM.
    if (/^setscale\s+-?\d+(?:\.\d+)?\s+-?\d+(?:\.\d+)?\s+-?\d+(?:\.\d+)?$/i.test(command)
      || /^(?:playsnd|loopsnd|stopsnd)\s+\S+(?:\s+\S+)*$/i.test(command)
      || /^(?:loop|makevalve)$/i.test(command)
      || /^quake\s+-?\d+(?:\.\d+)?(?:\s+-?\d+(?:\.\d+)?)?$/i.test(command)
      || /^sethitpoints\s+-?\d+(?:\.\d+)?$/i.test(command)) return;
    this.report(command, source);
  }

  private report(command: string, source: string): void {
    const key = `${source}:${command}`;
    if (!this.reported.has(key)) { this.reported.add(key); this.host.unsupported(command, source); }
  }
}
