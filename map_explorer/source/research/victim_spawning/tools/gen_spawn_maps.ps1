# gen_spawn_maps.ps1 -- one top-down spawn-point map per level, for handing a
# route-planning report to a speedrunner.
#
# Per .bsp: parses the entity lump, plots every entity as a faint grey dot
# (rough level footprint), then draws each RCS_VictimRescue's authored spawn
# spots as colour-coded circles -- spot 0 (editor origin) filled, spots 1..N
# (RCS_VictimAltSpot markers, one picked at random each load) ringed, joined by
# a dashed line. Legend lists each victim by name/type/spot-count.
#
# Output: tools\spawn_maps\<map>.svg  (+ spawns.json with every coordinate).
# SVG = scalable, opens in any browser. Ask if you want PNGs too.

param(
    [string]$MapsDir = "C:\fdny-decomp\FDNY SP 1.1\Data\Maps",
    [string]$OutDir  = "C:\fdny-decomp\tools\spawn_maps"
)

$PALETTE = @('#e6194b', '#3cb44b', '#4363d8', '#f58231', '#911eb4', '#00b8c4',
    '#f032e6', '#9A6324', '#808000', '#000075', '#e08214', '#4d9221',
    '#c51b7d', '#01665e', '#8073ac')

$KEY_RE = [regex]'^[A-Za-z_%][\w%]*$'

function Parse-Vec3([string]$s) {
    if (-not $s) { return $null }
    $p = ($s -replace ',', ' ') -split '\s+' | Where-Object { $_ -ne '' }
    if ($p.Count -lt 3) { return $null }
    try { return @([double]$p[0], [double]$p[1], [double]$p[2]) } catch { return $null }
}

function Get-Entities([string]$path) {
    $bytes = [System.IO.File]::ReadAllBytes($path)
    $enc = [System.Text.Encoding]::GetEncoding(28591)
    $s = $enc.GetString($bytes)
    $lo = $s.IndexOf('classname'); if ($lo -lt 0) { return @() }
    $lo = [Math]::Max(0, $lo - 64)
    $hi = $s.LastIndexOf('%name%'); if ($hi -lt 0) { return @() }
    $hi = [Math]::Min($bytes.Length, $hi + 128)

    $tok = New-Object System.Collections.ArrayList
    $p = $lo
    while ($p + 5 -le $hi) {
        $ln = [System.BitConverter]::ToUInt32($bytes, $p)
        if ($ln -ge 1 -and $ln -le 512 -and ($p + 4 + $ln) -le $hi -and $bytes[$p + 4 + $ln - 1] -eq 0) {
            $ok = $true
            for ($q = $p + 4; $q -lt $p + 4 + $ln - 1; $q++) {
                $b = $bytes[$q]
                if ($b -lt 9 -or ($b -gt 13 -and $b -lt 32) -or $b -gt 126) { $ok = $false; break }
            }
            if ($ok) {
                [void]$tok.Add($enc.GetString($bytes, $p + 4, $ln - 1).Trim())
                $p += 4 + $ln
                continue
            }
        }
        $p++
    }

    $ents = New-Object System.Collections.ArrayList
    $cur = New-Object System.Collections.ArrayList
    foreach ($t in $tok) {
        [void]$cur.Add($t)
        if ($cur.Count -ge 2 -and $cur[$cur.Count - 2] -eq '%name%') {
            $d = @{ '_id' = $cur[$cur.Count - 1] }
            if ($cur.Count -gt 2) {
                $kvs = $cur.GetRange(0, $cur.Count - 2)
                for ($j = 0; $j -lt $kvs.Count - 1; $j++) {
                    $k = $kvs[$j]
                    if ($KEY_RE.IsMatch($k) -and -not $d.ContainsKey($k)) { $d[$k] = $kvs[$j + 1]; $j++ }
                }
            }
            [void]$ents.Add($d)
            $cur.Clear()
        }
    }
    return $ents
}

function Get-Pos($d) {
    foreach ($k in 'location', 'origin', 'ZZ_location') {
        if ($d.ContainsKey($k)) { $v = Parse-Vec3 $d[$k]; if ($v) { return $v } }
    }
    return $null
}

# Pull the level's wall geometry from the GBSP lumps: VERTS (type 14, vec3),
# VERT_INDEX (13, int32), FACES (11, 36 bytes -> [0]=firstVert [1]=numVerts).
# Keep faces whose normal is near-horizontal (walls, not floors/ceilings),
# project their edges to the X/Z plane, de-dup. Returns @( @(x1,z1,x2,z2) ... ).
function Get-Geometry([string]$path) {
    $b = [System.IO.File]::ReadAllBytes($path)
    $off = @{}
    $p = 0
    for ($i = 0; $i -lt 60 -and $p + 12 -le $b.Length; $i++) {
        $t = [BitConverter]::ToInt32($b, $p); $s = [BitConverter]::ToInt32($b, $p + 4); $e = [BitConverter]::ToInt32($b, $p + 8)
        if ($t -eq 65535) { break }
        if ($s -lt 0 -or $e -lt 0 -or [int64]$s * $e -gt $b.Length) { break }
        if ($t -in 11, 13, 14) { $off[$t] = @(($p + 12), $e) }
        $p += 12 + [int64]$s * $e
    }
    if (-not ($off.ContainsKey(11) -and $off.ContainsKey(13) -and $off.ContainsKey(14))) { return @() }

    $nV = $off[14][1]; $vxyz = New-Object 'single[]' ($nV * 3)
    [Buffer]::BlockCopy($b, $off[14][0], $vxyz, 0, $nV * 12)
    $nI = $off[13][1]; $vidx = New-Object 'int32[]' $nI
    [Buffer]::BlockCopy($b, $off[13][0], $vidx, 0, $nI * 4)
    $nF = $off[11][1]; $fbase = $off[11][0]

    $segs = New-Object System.Collections.Generic.List[double[]]
    $seen = New-Object 'System.Collections.Generic.HashSet[string]'
    for ($fi = 0; $fi -lt $nF; $fi++) {
        $fo = $fbase + $fi * 36
        $first = [BitConverter]::ToInt32($b, $fo)
        $nv = [BitConverter]::ToInt32($b, $fo + 4)
        if ($nv -lt 3 -or $nv -gt 64 -or $first -lt 0 -or $first + $nv -gt $nI) { continue }
        # Newell normal
        $nx = 0.0; $ny = 0.0; $nz = 0.0
        for ($k = 0; $k -lt $nv; $k++) {
            $ai = $vidx[$first + $k] * 3; $ci = $vidx[$first + (($k + 1) % $nv)] * 3
            $ax = $vxyz[$ai]; $ay = $vxyz[$ai + 1]; $az = $vxyz[$ai + 2]
            $cx = $vxyz[$ci]; $cy = $vxyz[$ci + 1]; $cz = $vxyz[$ci + 2]
            $nx += ($ay - $cy) * ($az + $cz)
            $ny += ($az - $cz) * ($ax + $cx)
            $nz += ($ax - $cx) * ($ay + $cy)
        }
        $ln = [Math]::Sqrt($nx * $nx + $ny * $ny + $nz * $nz)
        if ($ln -le 0) { continue }
        if ([Math]::Abs($ny / $ln) -ge 0.30) { continue }   # floor/ceiling -> skip
        # one clean segment per wall face: the two verts farthest apart in X/Z
        $bx1 = 0.0; $bz1 = 0.0; $bx2 = 0.0; $bz2 = 0.0; $bd = -1.0
        for ($a = 0; $a -lt $nv; $a++) {
            $pa = $vidx[$first + $a] * 3; $pax = $vxyz[$pa]; $paz = $vxyz[$pa + 2]
            for ($c = $a + 1; $c -lt $nv; $c++) {
                $pc = $vidx[$first + $c] * 3
                $dx = $vxyz[$pc] - $pax; $dz = $vxyz[$pc + 2] - $paz
                $dd = $dx * $dx + $dz * $dz
                if ($dd -gt $bd) { $bd = $dd; $bx1 = $pax; $bz1 = $paz; $bx2 = $vxyz[$pc]; $bz2 = $vxyz[$pc + 2] }
            }
        }
        if ($bd -lt 64) { continue }   # < 8 units -> trim / rivet detail, skip
        $rx1 = [Math]::Round($bx1 / 3) * 3; $rz1 = [Math]::Round($bz1 / 3) * 3
        $rx2 = [Math]::Round($bx2 / 3) * 3; $rz2 = [Math]::Round($bz2 / 3) * 3
        $key = if ($rx1 -lt $rx2 -or ($rx1 -eq $rx2 -and $rz1 -le $rz2)) { "$rx1,$rz1,$rx2,$rz2" } else { "$rx2,$rz2,$rx1,$rz1" }
        if ($seen.Add($key)) { $segs.Add(@($bx1, $bz1, $bx2, $bz2)) }
    }
    return $segs
}

function N($v) { return ("{0:0.##}" -f [double]$v) }
function Esc([string]$t) { return ($t -replace '&', '&amp;' -replace '<', '&lt;' -replace '>', '&gt;') }

# --------------------------------------------------------------------------- #
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$jsonAll = [ordered]@{}
$svgList = New-Object System.Collections.ArrayList

foreach ($f in (Get-ChildItem $MapsDir -Filter *.bsp | Sort-Object Name)) {
    $map = [System.IO.Path]::GetFileNameWithoutExtension($f.Name).ToLower()
    $ents = Get-Entities $f.FullName

    $victims = @($ents | Where-Object { $_['classname'] -eq 'RCS_VictimRescue' })
    if ($victims.Count -eq 0) { Write-Host ("{0,-12} -- no victims, skipped" -f $map); continue }

    $altByV = @{}
    foreach ($e in $ents) {
        if ($e['classname'] -ne 'RCS_VictimAltSpot') { continue }
        $vid = $e['A_Victim']; $loc = Get-Pos $e
        if ($vid -and $loc) {
            if (-not $altByV.ContainsKey($vid)) { $altByV[$vid] = New-Object System.Collections.ArrayList }
            [void]$altByV[$vid].Add($loc)
        }
    }

    # victim records: name, type, ordered spot list
    $vrecs = New-Object System.Collections.ArrayList
    foreach ($v in $victims) {
        $o = Get-Pos $v
        $spots = New-Object System.Collections.ArrayList
        if ($o) { [void]$spots.Add($o) }
        if ($altByV.ContainsKey($v['_id'])) { foreach ($a in $altByV[$v['_id']]) { [void]$spots.Add($a) } }
        $nm = if ($v['A_Name']) { $v['A_Name'] } else { '<null>' }
        [void]$vrecs.Add(@{ name = $nm; type = $v['D_Type']; spots = $spots })
    }

    # level wall geometry (top-down)
    $walls = Get-Geometry $f.FullName

    # spawn-spot extent (always shown in full)
    $vx = @(); $vz = @(); $vy = @()
    foreach ($vr in $vrecs) { foreach ($sp in $vr.spots) { $vx += $sp[0]; $vz += $sp[2]; $vy += $sp[1] } }
    $sxmin = ($vx | Measure-Object -Minimum).Minimum; $sxmax = ($vx | Measure-Object -Maximum).Maximum
    $szmin = ($vz | Measure-Object -Minimum).Minimum; $szmax = ($vz | Measure-Object -Maximum).Maximum
    $ymin = ($vy | Measure-Object -Minimum).Minimum; $ymax = ($vy | Measure-Object -Maximum).Maximum

    # frame = whole level geometry (trimmed against strays) UNION the spawn extent,
    # so the building is always visible and no spot is ever off-frame.
    function Pctl($arr, $p) {
        $s = @($arr | Sort-Object); if ($s.Count -eq 0) { return $null }
        $i = [Math]::Max(0, [Math]::Min($s.Count - 1, [int][Math]::Round(($s.Count - 1) * $p))); return $s[$i]
    }
    $xmin = $sxmin; $xmax = $sxmax; $zmin = $szmin; $zmax = $szmax
    if ($walls.Count -gt 0) {
        $wx = New-Object System.Collections.Generic.List[double]; $wz = New-Object System.Collections.Generic.List[double]
        foreach ($s in $walls) { $wx.Add($s[0]); $wx.Add($s[2]); $wz.Add($s[1]); $wz.Add($s[3]) }
        $xmin = [Math]::Min($xmin, (Pctl $wx 0.01)); $xmax = [Math]::Max($xmax, (Pctl $wx 0.99))
        $zmin = [Math]::Min($zmin, (Pctl $wz 0.01)); $zmax = [Math]::Max($zmax, (Pctl $wz 0.99))
    }
    $padx = ($xmax - $xmin) * 0.03 + 64; $padz = ($zmax - $zmin) * 0.03 + 64
    $xmin -= $padx; $xmax += $padx; $zmin -= $padz; $zmax += $padz
    $xr = [Math]::Max(1, $xmax - $xmin); $zr = [Math]::Max(1, $zmax - $zmin)

    # fixed 1500 px on the long side -> every map renders at a consistent size,
    # chrome stays proportionate. Vector SVG still zooms for fine detail.
    $ppu = 1500.0 / [Math]::Max($xr, $zr)
    $plotW = $xr * $ppu; $plotH = $zr * $ppu
    $mL = 54; $mT = 76; $mB = 64; $gap = 26; $legW = 320
    $svgW = $mL + $plotW + $gap + $legW + 20
    $svgH = $mT + $plotH + $mB

    function PX($x) { return (N ($mL + ($x - $xmin) / $xr * $plotW)) }
    function PY($z) { return (N ($mT + ($zmax - $z) / $zr * $plotH)) }

    $step = 256.0
    while ($xr / $step -gt 11) { $step *= 2 }

    $sb = New-Object System.Text.StringBuilder
    [void]$sb.AppendLine("<svg xmlns='http://www.w3.org/2000/svg' width='$([int]$svgW)' height='$([int]$svgH)' font-family='Segoe UI, Arial, sans-serif'>")
    [void]$sb.AppendLine("<defs><clipPath id='plot'><rect x='$(N $mL)' y='$(N $mT)' width='$(N $plotW)' height='$(N $plotH)'/></clipPath></defs>")
    [void]$sb.AppendLine("<rect width='100%' height='100%' fill='#ffffff'/>")
    [void]$sb.AppendLine("<text x='$mL' y='34' font-size='22' font-weight='700' fill='#111'>$(Esc $map.ToUpper()) &#8212; victim spawn points</text>")
    [void]$sb.AppendLine("<text x='$mL' y='55' font-size='11.5' fill='#666'>top-down &#183; X right, Z up &#183; grey = walls &#183; filled circle = origin spawn, ring = alternate (game picks one)</text>")

    # grid
    [void]$sb.AppendLine("<g stroke='#e5e5e5' stroke-width='1'>")
    $gx = [Math]::Ceiling($xmin / $step) * $step
    while ($gx -le $xmax) { [void]$sb.AppendLine("<line x1='$(PX $gx)' y1='$(N $mT)' x2='$(PX $gx)' y2='$(N ($mT+$plotH))'/>"); $gx += $step }
    $gz = [Math]::Ceiling($zmin / $step) * $step
    while ($gz -le $zmax) { [void]$sb.AppendLine("<line x1='$(N $mL)' y1='$(PY $gz)' x2='$(N ($mL+$plotW))' y2='$(PY $gz)'/>"); $gz += $step }
    [void]$sb.AppendLine("</g>")
    [void]$sb.AppendLine("<rect x='$(N $mL)' y='$(N $mT)' width='$(N $plotW)' height='$(N $plotH)' fill='none' stroke='#999' stroke-width='1.5'/>")

    $mkR = 12.0; $mkF = $mkR * 0.95     # marker radius / label sizing base

    # level walls (clipped to the plot box); skip anything fully outside the view
    [void]$sb.AppendLine("<g clip-path='url(#plot)' stroke='#4d4d4d' stroke-width='0.9' stroke-opacity='0.9'>")
    $wdrawn = 0
    foreach ($s in $walls) {
        if (($s[0] -lt $xmin -and $s[2] -lt $xmin) -or ($s[0] -gt $xmax -and $s[2] -gt $xmax) -or
            ($s[1] -lt $zmin -and $s[3] -lt $zmin) -or ($s[1] -gt $zmax -and $s[3] -gt $zmax)) { continue }
        [void]$sb.AppendLine("<line x1='$(PX $s[0])' y1='$(PY $s[1])' x2='$(PX $s[2])' y2='$(PY $s[3])'/>")
        $wdrawn++
    }
    [void]$sb.AppendLine("</g>")

    # victims
    $jv = New-Object System.Collections.ArrayList
    $drawn = New-Object System.Collections.ArrayList     # (cx,cy) already placed -- nudge exact overlaps apart
    for ($i = 0; $i -lt $vrecs.Count; $i++) {
        $vr = $vrecs[$i]; $col = $PALETTE[$i % $PALETTE.Count]
        $scr = @()
        foreach ($sp in $vr.spots) {
            $cx = [double](PX $sp[0]); $cy = [double](PY $sp[2]); $ang = 0
            $nd = $mkR * 0.8
            while ($true) {
                $hit = $false
                foreach ($p in $drawn) { if ([Math]::Abs($p[0] - $cx) -lt $nd -and [Math]::Abs($p[1] - $cy) -lt $nd) { $hit = $true; break } }
                if (-not $hit -or $ang -ge 360) { break }
                $cx += ($mkR + 3) * [Math]::Cos($ang * [Math]::PI / 180); $cy += ($mkR + 3) * [Math]::Sin($ang * [Math]::PI / 180); $ang += 60
            }
            [void]$drawn.Add(@($cx, $cy)); $scr += , @($cx, $cy)
        }
        if ($vr.spots.Count -gt 1) {
            $d = "M " + (($scr | ForEach-Object { "$(N $_[0]) $(N $_[1])" }) -join " L ")
            [void]$sb.AppendLine("<path d='$d' fill='none' stroke='$col' stroke-width='2' stroke-dasharray='6,4' opacity='0.85'/>")
        }
        $L = [char](65 + ($i % 26))
        for ($k = 0; $k -lt $vr.spots.Count; $k++) {
            $sp = $vr.spots[$k]
            $cx = N $scr[$k][0]; $cy = N $scr[$k][1]
            $isOrigin = ($k -eq 0)
            $fillOp = if ($isOrigin) { '1' } else { '0.22' }
            $r = if ($isOrigin) { (N $mkR) } else { (N ($mkR * 0.86)) }
            $kind = if ($isOrigin) { "origin" } else { "alt spot $k" }
            $tip = Esc ("$L  $($vr.name) ($($vr.type)) - ${kind}: ($(N $sp[0]), $(N $sp[1]), $(N $sp[2]))")
            [void]$sb.AppendLine("<circle cx='$cx' cy='$cy' r='$r' fill='$col' fill-opacity='$fillOp' stroke='#1a1a1a' stroke-width='1.6'><title>$tip</title></circle>")
            $lab = if ($isOrigin) { "$L" } else { "$L$k" }
            $fs = if ($isOrigin) { (N ($mkF * 0.62)) } else { (N ($mkF * 0.5)) }
            $tc = if ($isOrigin) { '#fff' } else { '#1a1a1a' }
            [void]$sb.AppendLine("<text x='$cx' y='$(N ([double]$cy + $mkF * 0.22))' font-size='$fs' font-weight='700' text-anchor='middle' fill='$tc'>$lab</text>")
        }
        # json
        $jspots = New-Object System.Collections.ArrayList
        for ($k = 0; $k -lt $vr.spots.Count; $k++) {
            [void]$jspots.Add([ordered]@{ spot = $k; x = [Math]::Round($vr.spots[$k][0], 2); y = [Math]::Round($vr.spots[$k][1], 2); z = [Math]::Round($vr.spots[$k][2], 2) })
        }
        [void]$jv.Add([ordered]@{ label = "$L"; index = $i; name = $vr.name; type = $vr.type; n_spots = $vr.spots.Count; spots = $jspots })
    }

    # legend
    $lx = $mL + $plotW + $gap
    [void]$sb.AppendLine("<text x='$lx' y='$($mT-6)' font-size='15' font-weight='700' fill='#111'>Victims ($($vrecs.Count))</text>")
    $ly = $mT + 14
    for ($i = 0; $i -lt $vrecs.Count; $i++) {
        $vr = $vrecs[$i]; $col = $PALETTE[$i % $PALETTE.Count]; $L = [char](65 + ($i % 26))
        $nm = $vr.name; if ($nm.Length -gt 26) { $nm = $nm.Substring(0, 25) + [char]0x2026 }
        $ty = if ($vr.type) { " ($($vr.type))" } else { "" }
        $na = $vr.spots.Count - 1
        $sc = if ($na -ge 1) { "origin + $na alt spot$(if($na -gt 1){'s'})" } else { "origin only (fixed)" }
        [void]$sb.AppendLine("<rect x='$lx' y='$($ly-11)' width='14' height='14' fill='$col' stroke='#1a1a1a' stroke-width='1'/>")
        [void]$sb.AppendLine("<text x='$($lx+22)' y='$ly' font-size='12.5' fill='#111'><tspan font-weight='700'>$L</tspan>&#160; $(Esc $nm)$(Esc $ty)</text>")
        [void]$sb.AppendLine("<text x='$($lx+22)' y='$($ly+15)' font-size='10.5' fill='#777'>$sc</text>")
        $ly += 38
    }
    $ly += 6
    [void]$sb.AppendLine("<line x1='$lx' y1='$ly' x2='$($lx+$legW-10)' y2='$ly' stroke='#ddd'/>")
    $ly += 20
    foreach ($ln in @(
            "LETTER on a filled circle  = spawn origin",
            "LETTER+number on a ring    = alternate spot",
            "  e.g.  B = victim B's origin,  B1 / B2 = its alts",
            "the game picks ONE spot per victim at random",
            "dashed line links a victim's own spots",
            "grey = level walls (top-down)",
            "scale $([Math]::Round($ppu,3)) px/unit  &#183;  grid $([int]$step) units",
            "spawn span  X [$([int]$sxmin), $([int]$sxmax)]  Z [$([int]$szmin), $([int]$szmax)]",
            "spawn height Y [$([int]$ymin), $([int]$ymax)]  (not shown)")) {
        [void]$sb.AppendLine("<text x='$lx' y='$ly' font-size='10.5' fill='#666'>$ln</text>")
        $ly += 16
    }

    [void]$sb.AppendLine("</svg>")
    $outSvg = Join-Path $OutDir "$map.svg"
    [System.IO.File]::WriteAllText($outSvg, $sb.ToString(), [System.Text.UTF8Encoding]::new($false))
    [void]$svgList.Add(@{ map = $map; n = $vrecs.Count; svg = $sb.ToString() })

    $jsonAll[$map] = [ordered]@{
        bounds  = [ordered]@{ x = @([Math]::Round($xmin, 1), [Math]::Round($xmax, 1)); z = @([Math]::Round($zmin, 1), [Math]::Round($zmax, 1)); y = @([Math]::Round($ymin, 1), [Math]::Round($ymax, 1)) }
        victims = $jv
    }
    Write-Host ("{0,-12} {1,2} victims -> {2}" -f $map, $vrecs.Count, (Split-Path $outSvg -Leaf))
}

[System.IO.File]::WriteAllText((Join-Path $OutDir "spawns.json"), ($jsonAll | ConvertTo-Json -Depth 9), [System.Text.UTF8Encoding]::new($false))
Write-Host "`nwrote $(Join-Path $OutDir 'spawns.json')"

# contact-sheet report (all maps inline, for handing off)
$h = New-Object System.Text.StringBuilder
[void]$h.AppendLine("<!doctype html><html><head><meta charset='utf-8'><title>FDNY victim spawn maps</title>")
[void]$h.AppendLine("<style>body{font:14px/1.5 'Segoe UI',Arial,sans-serif;margin:24px;color:#111;background:#fafafa}h1{margin:0 0 4px}p.sub{color:#666;margin:0 0 18px}nav{margin:0 0 24px}nav a{display:inline-block;margin:0 10px 6px 0;padding:3px 9px;background:#eee;border-radius:4px;text-decoration:none;color:#333}section{background:#fff;border:1px solid #e2e2e2;border-radius:8px;padding:14px;margin:0 0 22px}svg{max-width:100%;height:auto;border:1px solid #eee}</style></head><body>")
[void]$h.AppendLine("<h1>F.D.N.Y. Firefighter &#8212; victim spawn maps</h1>")
[void]$h.AppendLine("<p class='sub'>Top-down, per level. Grey = level walls. Each victim = one colour and one letter. A <b>filled circle with the bare letter</b> (e.g. <b>B</b>) is that victim's origin spawn; a <b>ring with letter+number</b> (<b>B1</b>, <b>B2</b>) is an alternate. The game picks ONE spot per victim at random each load; the dashed line links a victim's options. Coordinates in <code>spawns.json</code>. Tell me which spots are the good ones.</p>")
[void]$h.Append("<nav>")
foreach ($it in $svgList) { [void]$h.Append("<a href='#$($it.map)'>$($it.map) ($($it.n))</a>") }
[void]$h.AppendLine("</nav>")
foreach ($it in $svgList) {
    [void]$h.AppendLine("<section id='$($it.map)'>")
    [void]$h.AppendLine(($it.svg -replace "<\?xml[^>]*\?>", ""))
    [void]$h.AppendLine("</section>")
}
[void]$h.AppendLine("</body></html>")
[System.IO.File]::WriteAllText((Join-Path $OutDir "index.html"), $h.ToString(), [System.Text.UTF8Encoding]::new($false))
Write-Host "wrote $(Join-Path $OutDir 'index.html')"

# rasterise each SVG to PNG via headless Edge/Chrome, if one is installed
$browser = @(
    "$env:ProgramFiles\Microsoft\Edge\Application\msedge.exe",
    "${env:ProgramFiles(x86)}\Microsoft\Edge\Application\msedge.exe",
    "$env:ProgramFiles\Google\Chrome\Application\chrome.exe",
    "${env:ProgramFiles(x86)}\Google\Chrome\Application\chrome.exe"
) | Where-Object { Test-Path $_ } | Select-Object -First 1
if ($browser) {
    foreach ($svg in (Get-ChildItem $OutDir -Filter *.svg)) {
        [xml]$x = Get-Content $svg.FullName -Raw
        $w = [int]$x.svg.width; $ht = [int]$x.svg.height
        $png = [System.IO.Path]::ChangeExtension($svg.FullName, ".png")
        & $browser --headless=new --disable-gpu --hide-scrollbars --force-device-scale-factor=1.5 `
            "--screenshot=$png" "--window-size=$w,$ht" ("file:///" + ($svg.FullName -replace '\\', '/')) 2>&1 | Out-Null
    }
    Start-Sleep 1
    Write-Host "wrote $((Get-ChildItem $OutDir -Filter *.png).Count) PNGs"
} else {
    Write-Host "no Edge/Chrome found -- SVGs only (open in a browser)"
}
