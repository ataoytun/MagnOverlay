# MagnOverlay

A PoC that hijacks Windows Magnifier to create a transparent overlay without injecting code into protected processes.

> **Note:** This is a PoC with spaghetti code that demonstrates the technique, it's not meant to be production quality.

<p align="center">
  <img src="Assets/1.gif" alt="1.gif" width="50%">
</p>

## How It Works

This project hijacks Windows Magnifier to create an overlay that works on top of any application, including games with anticheat protection:

1. Launches Windows Magnifier
2. Finds and hijacks its window by modifying properties
3. Makes the window transparent, layered, and topmost*
4. Renders content with DirectX 11 and ImGui

Key advantages:
- No code injection into protected processes
- No API hooking
- No game file modifications
- Works without directly interacting with protected processes

## Implementation Details

```cpp
HWND MagnifierOverlay::FindMagnifierWindow() {
    const char* magnifierClasses[] = {
        "Magnifier",
        "MagUIClass",
        "Screen Magnifier Fullscreen Window"
    };

    return Utils::FindWindowByClasses(magnifierClasses, 3);
}
```

Window style modifications:

```cpp
SetWindowLongPtr(m_magnifierWindow, GWL_STYLE, originalStyle | WS_VISIBLE);
SetWindowLongPtr(m_magnifierWindow, GWL_EXSTYLE, (originalExStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT) & ~WS_EX_NOREDIRECTIONBITMAP);
SetLayeredWindowAttributes(m_magnifierWindow, m_transparencyColor, 0, LWA_COLORKEY);
```

## Detection Vectors

This method avoids many common detection vectors, but could still be detected through:

1. Magnifier window style modifications
2. Window transparency attributes
3. Extended window styles (WS_EX_LAYERED, WS_EX_TRANSPARENT)
4. Magnifier process monitoring
5. Unusual window z-order placement
