# Backdoor scanner

Introducing a C++ console application designed specifically for scanning Lua scripts, aimed at detecting potential security vulnerabilities like backdoors, suspicious links, and obfuscated code. This tool is invaluable for FiveM developers seeking to protect their Lua scripts from malicious threats.

---

## **Core Features**

- **Backdoor Analysis**: Effectively identifies suspicious functions and patterns commonly associated with backdoor scripts.
- **Link Inspection**: Scans scripts for links, ignoring known safe domains such as `discord.gg` and `discord.webhook`.
- **Obfuscation Recognition**: Detects obfuscated or encoded code that may conceal harmful functions.
- **Comprehensive Logging**: Provides detailed reports on findings, including file paths, line numbers, and specific issues detected.
- **Automatic Cleaning**: Offers a feature to automatically remove detected malicious code from scripts.

---
## **Operational Workflow**

### **Step-by-Step Procedure**

1. **Execute the Application**  
   Begin by running the compiled program.

2. **Select a Scanning Method**  
   - **Option 1**: Reviews all Lua scripts in the `scripts` folder, logging any suspicious findings in the `output` directory while keeping the original files unchanged.
   - **Option 2**: Scans for and eliminates harmful code from Lua scripts in the `scripts` directory, saving the cleaned versions in the `fixed` directory and documenting all changes.

3. **Detection and Logging Process**  
   - The scanner thoroughly checks each line in every Lua file for:
     - **Suspicious URLs** (excluding common safe links like `discord.gg` and `discord.com/api/webhooks/`)
     - **Backdoor Indicators** (e.g., `loadstring`, `RunCode`, `decode`)
     - **Obfuscated Code** or other potentially dangerous functions.
   - When problematic code is identified:
     - **Option 1**: Logs the file path, line number, flagged code, and the reason for the flag in a log file within the `output` directory.
     - **Option 2**: Removes the flagged code from the file, stores the cleaned version in the `fixed` directory, and logs every detail of the removal.

4. **Reviewing Logs and Results**  
   - After the scanning process, check the contents of the `output` or `fixed` directories:
     - **Logs from Option 1**: Provides a detailed report of flagged code in `output`, without altering the original scripts.
     - **Logs and Cleaned Files from Option 2**: Stored in `fixed`, these logs document all modifications made to each file.

---


### Detailed Log Example (in `output` or `fixed`)
```plaintext
Filename: SomeScript.lua
Line 45: loadstring("encoded_string") (Reason: Obfuscated code detected)
Line 78: https://somesite.com/link (Reason: Suspicious URL detected)
```

Each log entry includes:
- **Filename**: The Lua file scanned.
- **Line Number**: The line where the issue was found.
- **Code Snippet**: The specific code flagged.
- **Reason**: Why it was flagged (e.g., "Obfuscated code detected").

---

## FAQ

### Why does my antivirus flag this as a trojan?

Due to certain scanning and file-modifying functions, antivirus software might flag this application as a potential threat. This is a false positive. To avoid this, we recommend:

1. Running the application in a safe environment.
2. Reviewing the code to ensure it aligns with your security standards.


---

## Contributing

Contributions are welcome! Feel free to submit pull requests to improve detection, add new features, or resolve issues.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

---

## Contact

For questions, reach out to the creator:

- **P4 Scripts**: [Join Discord](https://discord.gg/uEuetEY3jd)

