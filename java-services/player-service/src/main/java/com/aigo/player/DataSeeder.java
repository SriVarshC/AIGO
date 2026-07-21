package com.aigo.player;

import com.aigo.player.model.Player;
import com.aigo.player.repository.PlayerRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.stereotype.Component;

@Component
public class DataSeeder implements CommandLineRunner {

    @Autowired
    private PlayerRepository playerRepository;

    @Override
    public void run(String... args) throws Exception {
        playerRepository.save(new Player("Titan", "US-EAST", 1805, 53, 33, 20, 642, 318));
        playerRepository.save(new Player("Nexus", "US-EAST", 1665, 39, 23, 16, 498, 271));
        playerRepository.save(new Player("Surge", "EU-WEST", 1428, 26, 12, 14, 301, 289));
        playerRepository.save(new Player("Echo",  "EU-WEST", 1421, 32, 16, 16, 387, 352));
        playerRepository.save(new Player("Nova",  "AP-EAST", 1202, 23, 11, 12, 241, 258));
        playerRepository.save(new Player("Byte",  "AP-EAST", 1188, 19,  9, 10, 198, 221));
        System.out.println("[AIGO] Player data seeded - 6 players loaded");
    }
}