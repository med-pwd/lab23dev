# 🔐 LAB 23 — JNI + Protection Anti-Debug Native (Android)

---
analyste:souaid med amine
# 📌 Description

Ce projet Android démontre l’utilisation de **JNI (Java Native Interface)** afin d’intégrer du code natif C++ dans une application Android.

Le laboratoire met également en œuvre une première couche de **protection anti-debug native** permettant de détecter si l’application est exécutée sous un debugger.

Le projet utilise :

* Android Studio
* Android NDK
* CMake
* JNI
* C++
* Logcat

---

# 🎯 Objectifs pédagogiques

À la fin de ce laboratoire, l’étudiant sera capable de :

* Comprendre le fonctionnement de JNI
* Communiquer entre Java et C++
* Configurer Android NDK
* Compiler une bibliothèque native avec CMake
* Implémenter une détection anti-debug simple
* Lire les logs de sécurité via Logcat
* Comprendre les bases du hardening Android natif

---

# 📱 Fonctionnalités

Le projet permet :

* le chargement d’une bibliothèque native (`native-lib`)
* l’appel de fonctions C++ depuis Java
* la détection d’un debugger via `TracerPid`
* l’affichage d’un état de sécurité
* la journalisation des événements dans Logcat

---

# 🏗 Architecture du projet

```text
app/
 └── src/
      └── main/
           ├── java/com/example/jnidemo/
           │     └── MainActivity.java
           │
           ├── cpp/
           │     ├── native-lib.cpp
           │     └── CMakeLists.txt
           │
           └── res/layout/
                 └── activity_main.xml
```

---

# ⚙️ Configuration CMake

Fichier :

```text
app/src/main/cpp/CMakeLists.txt
```

Code :

```cmake
cmake_minimum_required(VERSION 3.22.1)

project("jnidemo")

add_library(
        native-lib
        SHARED
        native-lib.cpp)

find_library(
        log-lib
        log)

target_link_libraries(
        native-lib
        ${log-lib})
```

---

# 💻 Code natif C++ (JNI)

Fichier :

```text
native-lib.cpp
```

Code :

```cpp
#include <jni.h>
#include <string>
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "JNI_LOG", __VA_ARGS__)

// Détection debugger simple
bool isDebuggerAttached() {

    FILE *file = fopen("/proc/self/status", "r");

    if (file) {

        char line[256];

        while (fgets(line, sizeof(line), file)) {

            if (strncmp(line, "TracerPid:", 10) == 0) {

                int pid = atoi(line + 10);

                fclose(file);

                return pid != 0;
            }
        }

        fclose(file);
    }

    return false;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_checkSecurity(JNIEnv *env, jobject) {

    if (isDebuggerAttached()) {

        LOGD("⚠ Debugger detected!");

        return env->NewStringUTF("Security Alert: Debugger detected");
    }

    LOGD("✔ No debugger detected");

    return env->NewStringUTF("App is secure");
}
```

---

# 📱 Code Java — MainActivity

```java
public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    public native String checkSecurity();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.textView);

        tv.setText(checkSecurity());
    }
}
```

---

# 📊 Utilisation de Logcat

Filtrer avec :

```text
JNI_LOG
```

Résultats possibles :

```text
✔ No debugger detected
```

ou

```text
⚠ Debugger detected!
```

---

# 🔐 Bonnes pratiques sécurité

* éviter toute logique sensible uniquement en Java ;
* utiliser plusieurs couches de sécurité ;
* combiner anti-debug + obfuscation ;
* protéger les fonctions natives importantes.

---

# ⚠️ Limites

Cette protection :

* reste simple ;
* peut être contournée ;
* ne protège pas totalement contre le reverse engineering.

Des outils comme Frida ou le hooking JNI peuvent contourner cette détection.

---

# 🚀 Améliorations possibles

* Détection Frida
* Anti-root
* Vérification intégrité APK
* Obfuscation native
* Checksum binaire

---

# 🛠 Technologies utilisées

| Technologie    | Rôle                   |
| -------------- | ---------------------- |
| Android Studio | IDE Android            |
| Java           | Interface Android      |
| C++            | Code natif             |
| JNI            | Communication Java/C++ |
| NDK            | Compilation native     |
| CMake          | Build natif            |
| Logcat         | Logs Android           |

---
<img width="337" height="646" alt="image" src="https://github.com/user-attachments/assets/3f566da6-1b4d-4bab-926c-97f79823b1fd" />


