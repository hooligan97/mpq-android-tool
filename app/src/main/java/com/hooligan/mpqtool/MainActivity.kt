package com.hooligan.mpqtool

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val textView: TextView = findViewById(R.id.sample_text)
        textView.text = "StormLib Native Engine Ready"
    }

    external fun extractMpqFile(mpqPath: String, fileName: String, outputPath: String): Boolean

    companion object {
        init {
            System.loadLibrary("mpqtool")
        }
    }
}
