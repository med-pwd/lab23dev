package com.example.jni;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "AntiDebugJava";

    // Load the native library
    static {
        System.loadLibrary("native-lib");
    }

    // Declare the native method
    public native boolean isDebugDetected();

    private TextView statusText;
    private TextView detailText;
    private Button scanButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        // Handle Window Insets
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        // Initialize UI
        statusText = findViewById(R.id.statusText);
        detailText = findViewById(R.id.detailText);
        scanButton = findViewById(R.id.scanButton);

        scanButton.setOnClickListener(v -> runSecurityCheck());
    }

    private void runSecurityCheck() {
        Log.i(TAG, "Starting security scan...");
        
        boolean isDetected = isDebugDetected();

        if (isDetected) {
            statusText.setText("THREAT DETECTED");
            statusText.setTextColor(Color.RED);
            detailText.setText("Debugger or Tracer detected! Application execution restricted.");
            Toast.makeText(this, "Security Alert: Debugging detected!", Toast.LENGTH_LONG).show();
            Log.w(TAG, "Security check FAILED: Debugger detected.");
        } else {
            statusText.setText("SYSTEM SECURE");
            statusText.setTextColor(Color.GREEN);
            detailText.setText("No active debuggers detected. Environment is safe.");
            Toast.makeText(this, "Security check PASSED", Toast.LENGTH_SHORT).show();
            Log.i(TAG, "Security check PASSED.");
        }
    }
}