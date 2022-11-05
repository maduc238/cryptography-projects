package com.maduc.salsa20;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class AppController {
    @GetMapping("/")
    public String index() {
        return "index";
    }

    @GetMapping("/about")
    public String about() {
        return "about";
    }

    @GetMapping("/result")
    public String result(
            @RequestParam(name = "plaintext", required = true, defaultValue = "") String plaintext,
            @RequestParam(name = "p_hex", required = false, defaultValue = "") String p_hex,
            @RequestParam(name = "key", required = true, defaultValue = "") String key,
            @RequestParam(name = "k_hex", required = false, defaultValue = "") String k_hex,
            @RequestParam(name = "nonce", required = true, defaultValue = "") String nonce,
            @RequestParam(name = "n_hex", required = false, defaultValue = "") String n_hex,
            @RequestParam(name = "vehicle1", required = false, defaultValue = "") String vehicle1,
            String ciphertext, String cipherhex,
            Model model
    ) {
        if(p_hex.equals("true")) {
            // đọc dạng hex
            model.addAttribute("plainhex", plaintext);
            plaintext = Functions.convertToText(plaintext);
            model.addAttribute("plaintext", plaintext);
            // plainhex
        } else {
            // đọc dạng text
            model.addAttribute("plaintext", plaintext);
            model.addAttribute("plainhex", Functions.convertToHex(plaintext));
        }
        
        if (k_hex.equals("true")) {
            key = Functions.convertToText(key);
        }
        if(key.length() != 16 && key.length() != 32) return "false";

        if (n_hex.equals("true")) {
            nonce = Functions.convertToText(nonce);
        }
        if(nonce.length() != 8) return "false";

        Salsa20 sa = new Salsa20(key, nonce);
        ciphertext = sa.encrypt(plaintext);
        cipherhex = Functions.convertToHex(ciphertext);
        model.addAttribute("ciphertext", ciphertext);
        model.addAttribute("cipherhex", cipherhex);
        return "result";
    }
}
