# Fog-Computing-Network-Simulation

# 🧩 INET Installation Guide for OMNeT++ 6.2 (WSL)

This guide explains how to **install and build the INET Framework** (latest version from GitHub) to use with **OMNeT++ 6.2** on **WSL 2 Windows**.

Follow these steps carefully — they ensure INET is properly compiled, imported, and linked to your project so that you can use INET modules such as **mobility**, **wireless**, and **visualization** components.

---

## 🧭 Step 1: Open the OMNeT++ Environment

In your WSL terminal, navigate to your OMNeT++ installation directory and load the environment variables:

```bash
cd ~/default_workspace/omnetpp-6.2.0
source setenv
```

You should see:

```
Environment for 'omnetpp-6.2.0' ... is ready.
```

✅ This step ensures that commands like `opp_makemake` and `omnetpp` are available globally in your shell session.

---

## 📦 Step 2: Download the INET Framework

From your workspace root directory:

```bash
cd ~/default_workspace
git clone https://github.com/inet-framework/inet.git
cd inet
```

📝 This clones the latest master branch of INET, which is compatible with OMNeT++ 6.2.

---

## ⚙️ Step 3: Set Up the INET Build Environment

INET also has its own `setenv` script that needs to be sourced alongside OMNeT++'s environment.

Run both commands:

```bash
source ~/default_workspace/omnetpp-6.2.0/setenv
source setenv
```

Expected output:

```
Environment for 'inet' in directory '/home/opp_env/default_workspace/inet' is ready.
```

✅ This step ensures the INET environment is properly linked to OMNeT++.

---

## 🏗 Step 4: Build INET

Once the environment is set, you can build INET using:

```bash
make makefiles
make -j$(nproc)
```

When the build completes successfully, you'll see something similar to:

```
Creating shared library: ../out/gcc-release/src/libINET.so
```

✅ This indicates that INET compiled successfully and is ready for use.

---

## 🧰 Step 5: Import INET into OMNeT++ IDE

In the OMNeT++ IDE:

1. Go to **File → Import → Existing Project into Workspace**
2. Browse to: `~/default_workspace/inet`
3. Check ✅ **INET** and click **Finish**

This step imports INET as a project inside your OMNeT++ workspace so the IDE recognizes its NED and C++ files.

---

## 🔗 Step 6: Link INET to Your Project

To make your own project use INET's modules and libraries, link it in the IDE:

1. Right-click your project in the **Project Explorer**
2. Choose **Properties → Project References**
3. Check ✅ **inet**
4. Click **Apply and Close**

✅ After this step, your project can import INET packages, for example:

```ned
import inet.mobility.single.*;
import inet.visualizer.canvas.integrated.*;
```

---

## ⚙️ Step 7: Update Your NED Path (for CLI Simulations)

If you plan to run simulations from the terminal (not from the IDE), you must include INET's NED files in your project's `.ini` configuration file.

Add this to your `omnetpp.ini`:

```ini
[General]
ned-path = .:../simulations:../../inet/src
```

✅ This ensures OMNeT++ can locate INET's `.ned` definitions during simulation runs.

---

## 🧭 Step 8: (Optional) Rebuild or Clean if Needed

If you encounter missing references or build errors, run the following from your project directory:

```bash
make cleanall
make
```

This ensures that the IDE or command-line build system recognizes your environment and all linked libraries properly.

---

## 🧩 Step 9: Verify Project References (Critical Step)

In the OMNeT++ IDE, make sure the `inet` project is properly linked:

1. In the top menu, go to **Project → Properties**
2. Select **Project References** on the left sidebar
3. Confirm that ✅ **inet** is checked
4. Click **Apply and Close**

💡 If this step is skipped, your project will not compile even though INET is built.

---

## 📘 Step 10: Keep Your Environment Active

Every time you open a new WSL terminal, remember to re-load both environment scripts before running or building simulations:

```bash
source ~/default_workspace/omnetpp-6.2.0/setenv
source ~/default_workspace/inet/setenv
```

This ensures all paths and variables are properly configured.

---

## 📋 Summary Table

| Step | Command / Action | Purpose |
|------|------------------|---------|
| 1 | `source omnetpp-6.2.0/setenv` | Load OMNeT++ environment |
| 2 | `git clone https://github.com/inet-framework/inet.git` | Download INET |
| 3 | `source inet/setenv` | Initialize INET environment |
| 4 | `make makefiles && make -j$(nproc)` | Build INET library |
| 5 | Import INET via IDE | Add INET to workspace |
| 6 | Enable **Project → References → inet** | Link your project with INET |
| 7 | `ned-path = ../../inet/src` | Make INET visible in CLI runs |
| 8 | `make cleanall && make` | Rebuild your project if needed |
| 9 | Check **Project References** | Verify link between your project and INET |
| 10 | Import mobility/visualizer packages | Use INET modules in NED |
| 11 | Re-run `source setenv` scripts | Keep environment active in WSL |

---

## ✅ After Completing These Steps

- INET is installed, built, and linked with OMNeT++ 6.2
- You can now use INET's mobility, visualization, and networking modules in your simulation projects
- Your OMNeT++ workspace and WSL environment are fully ready for development and testing

---

## 🆘 Troubleshooting

If you encounter issues:

- **Build errors**: Run `make cleanall` in both INET and your project directories, then rebuild
- **Missing modules**: Verify Project References (Step 9) and ensure both `setenv` scripts are sourced
- **NED file not found**: Check your `ned-path` configuration in `omnetpp.ini`

---

## 📚 Additional Resources

- [INET Framework Documentation](https://inet.omnetpp.org/)
- [OMNeT++ Documentation](https://omnetpp.org/documentation/)
- [INET GitHub Repository](https://github.com/inet-framework/inet)
