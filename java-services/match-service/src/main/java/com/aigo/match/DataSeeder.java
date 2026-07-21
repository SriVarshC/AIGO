package com.aigo.match;

import com.aigo.match.model.Match;
import com.aigo.match.repository.MatchRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.stereotype.Component;
import java.time.LocalDateTime;

@Component
public class DataSeeder implements CommandLineRunner {

    @Autowired
    private MatchRepository matchRepository;

    @Override
    public void run(String... args) throws Exception {
        LocalDateTime base = LocalDateTime.of(2026, 6, 25, 18, 0, 0);
        matchRepository.save(new Match("MATCH_001", 5L, "Nova",  6L, "Byte",
            "Nova",  "Arena 1", 6, 180, base));
        matchRepository.save(new Match("MATCH_002", 4L, "Echo",  3L, "Surge",
            "Echo",  "Arena 2", 5, 150, base.plusMinutes(30)));
        matchRepository.save(new Match("MATCH_003", 2L, "Nexus", 1L, "Titan",
            "Nexus", "Arena 3", 6, 180, base.plusMinutes(60)));
        System.out.println("[AIGO] Match data seeded - 3 matches loaded");
    }
}