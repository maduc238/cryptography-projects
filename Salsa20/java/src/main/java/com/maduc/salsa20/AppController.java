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
            @RequestParam(name = "p_uni", required = false, defaultValue = "") String p_uni,
            @RequestParam(name = "key", required = true, defaultValue = "") String key,
            @RequestParam(name = "k_hex", required = false, defaultValue = "") String k_hex,
            @RequestParam(name = "k_uni", required = false, defaultValue = "") String k_uni,
            @RequestParam(name = "nonce", required = true, defaultValue = "") String nonce,
            @RequestParam(name = "n_hex", required = false, defaultValue = "") String n_hex,
            @RequestParam(name = "n_uni", required = false, defaultValue = "") String n_uni,
            String ciphertext, String cipherhex,
            Model model
    ) {
        Boolean puni = false, kuni = false, nuni = false;
        if(p_uni.equals("true")) puni = true;
        if(k_uni.equals("true")) kuni = true;
        if(n_uni.equals("true")) nuni = true;
        
        if(p_hex.equals("true")) {
            // đọc dạng hex
            model.addAttribute("plainhex", plaintext);
            plaintext = Functions.convertToText(plaintext, puni);
            model.addAttribute("plaintext", plaintext);
            // plainhex
        } else {
            // đọc dạng text
            model.addAttribute("plaintext", plaintext);
            model.addAttribute("plainhex", Functions.convertToHex(plaintext, puni));
        }
        if (puni) {
            plaintext = Functions.convertToText(Functions.convertToHex(plaintext, true), false);
        }
        
        if (k_hex.equals("true")) {
            key = Functions.convertToText(key, kuni);
        }
        else if (kuni) {
            key = Functions.convertToText(Functions.convertToHex(key, true), false);
        }
        if(key.length() != 16 && key.length() != 32) return "false";

        if (n_hex.equals("true")) {
            nonce = Functions.convertToText(nonce, nuni);
        }
        else if (nuni) {
            nonce = Functions.convertToText(Functions.convertToHex(nonce, true), false);
        }
        if(nonce.length() != 8) return "false2";

        Salsa20 sa = new Salsa20(key, nonce);

        ciphertext = sa.encrypt(plaintext);
        cipherhex = Functions.convertToHex(ciphertext, false);
        if (puni) ciphertext = Functions.convertToText(Functions.convertToHex(ciphertext, false), true);
        
        model.addAttribute("ciphertext", ciphertext);
        model.addAttribute("cipherhex", cipherhex);
        return "result";
    }
}
