# Agent Instructions — Unreal Interaction SDK Sample

C++ Unreal project demonstrating the Meta XR Interaction SDK plugin (hand- and controller-driven grab, poke, ray, distance grab, hand poses). Ships with two `.uproject` configurations — one OculusXR-backed, one OpenXR-backed — over the same C++ runtime module.

## Source-of-truth files (read these first, do not duplicate their contents in this file)

For setup, build steps, SDK versions, and project layout, read:

- `README.md` — official setup, plugin acquisition, Visual Studio configuration, troubleshooting
- `MetaIsdkSample.uproject` — OculusXR plugin variant (primary)
- `OpenXRIsdkSample.uproject` — OpenXR plugin variant
- `Source/MetaIsdkSample/` — C++ module shared by both variants
- `Config/` — Unreal config, input mappings
- `.gitattributes` — Git LFS (required for binary UAssets)
- `LICENSE` — license terms (Meta License; MIT only where explicitly marked)

## Quest / Horizon-specific notes

- C++ project — Visual Studio with the "Game development with C++" workload is required even to open the project. Right-click the `.uproject` -> Generate Visual Studio project files, then build. Opening the `.uproject` directly without compiling produces the "modules built with a different engine version" error.
- `Plugins/` is intentionally empty in the repo; the Meta XR plugin and the Meta XR Interaction SDK plugin must be downloaded from the developer site and unzipped in before the project will load. Keep `OculusXR` and `OculusInteraction` on matched versions and matched to the engine version.
- Two `.uproject` files exist (OculusXR vs OpenXR backends) — pick one consciously. Compile flags or behaviors specific to one backend may differ; verify both still build before changing module-wide settings.

# Meta Quest tooling

This is a Meta Quest / Horizon OS sample. The bespoke intro above is the source of truth for what this project is and how it's built — use it (and the files it points at) instead of restating facts from memory.

When the user asks anything about Quest device behavior, build / deploy / debug / capture flows, on-device performance, or Horizon OS APIs, reach for these tools instead of generic Unreal answers:

- **`hzdb`** — Quest-aware ADB wrapper (device list, install / launch / stop, logs, screenshots, Perfetto traces, on-device docs search). Already wired up as an MCP server via `.mcp.json`, `.vscode/mcp.json`, and `.cursor/mcp.json`. Also runnable directly: `npx -y @meta-quest/hzdb <subcommand>`.
- **Meta Quest Agentic Tools** — the full skill set, including Unreal-specific skills: <https://github.com/meta-quest/agentic-tools>. Install per your client (Claude Code: `/plugin install meta-vr@meta-quest`; Gemini CLI: `gemini extensions install https://github.com/meta-quest/agentic-tools`; Cursor / VS Code: install the **Meta Horizon** extension from the Marketplace).

A few behavior expectations:

- **Read this repo's files first.** Before answering anything project-specific, read `README.md` and whichever source-of-truth files the intro above points at. Don't restate their contents in chat — quote or link instead.
- **Use `hzdb` for device-side work.** Anything that touches an attached Quest (install, launch, logs, screenshot, capture, manifest inspection) goes through `hzdb`, not raw `adb`.
- **Check live Horizon OS docs before answering API questions.** `hzdb docs search "..."` queries the live docs; training data on Horizon OS APIs goes stale fast.
- **Don't fabricate SDK / engine versions.** If a version isn't visible in this repo's files, say so rather than guessing.
