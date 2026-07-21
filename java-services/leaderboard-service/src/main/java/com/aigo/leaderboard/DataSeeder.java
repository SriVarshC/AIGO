package com.aigo.leaderboard;

import com.aigo.leaderboard.model.LeaderboardEntry;
import com.aigo.leaderboard.repository.LeaderboardRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.stereotype.Component;

@Component
public class DataSeeder implements CommandLineRunner {

    @Autowired
    private LeaderboardRepository leaderboardRepository;

    @Override
    public void run(String... args) throws Exception {
        leaderboardRepository.save(new LeaderboardEntry(1L, "Titan", "US-EAST", 1805, "DIAMOND",  33, 20, 53));
        leaderboardRepository.save(new LeaderboardEntry(2L, "Nexus", "US-EAST", 1665, "PLATINUM", 23, 16, 39));
        leaderboardRepository.save(new LeaderboardEntry(3L, "Surge", "EU-WEST", 1428, "GOLD",     12, 14, 26));
        leaderboardRepository.save(new LeaderboardEntry(4L, "Echo",  "EU-WEST", 1421, "GOLD",     16, 16, 32));
        leaderboardRepository.save(new LeaderboardEntry(5L, "Nova",  "AP-EAST", 1202, "SILVER",   11, 12, 23));
        leaderboardRepository.save(new LeaderboardEntry(6L, "Byte",  "AP-EAST", 1188, "BRONZE",    9, 10, 19));
        System.out.println("[AIGO] Leaderboard data seeded - 6 entries loaded");
    }
}