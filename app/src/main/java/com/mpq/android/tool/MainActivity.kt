package com.mpq.android.tool

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.mpq.android.tool.databinding.ActivityMainBinding
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Initialize native library
        System.loadLibrary("mpq-core")

        setupUI()
    }

    private fun setupUI() {
        lifecycleScope.launch {
            // Initialize UI elements
            binding.apply {
                // TODO: Implement UI logic
            }
        }
    }
}
