package com.aigo.leaderboard.controller;

import com.aigo.leaderboard.model.LeaderboardEntry;
import com.aigo.leaderboard.service.LeaderboardService;
import com.aigo.leaderboard.service.LiveLeaderboardService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/leaderboard")
public class LeaderboardController {

    @Autowired
    private LeaderboardService leaderboardService;

    @Autowired
    private LiveLeaderboardService liveLeaderboardService;

    @GetMapping("/health")
    public Map<String, String> health() {
        Map<String, String> res = new LinkedHashMap<>();
        res.put("status", "UP");
        res.put("service", "leaderboard-service");
        res.put("port", "8083");
        return res;
    }

    @GetMapping
    public List<Map<String, Object>> getLeaderboard() {
        return leaderboardService.getRankedLeaderboard();
    }

    @GetMapping("/top/{n}")
    public List<Map<String, Object>> getTopN(@PathVariable int n) {
        return leaderboardService.getTopN(n);
    }

    @GetMapping("/region/{region}")
    public List<Map<String, Object>> getByRegion(@PathVariable String region) {
        return leaderboardService.getLeaderboardByRegion(region);
    }

    @GetMapping("/player/{playerId}")
    public ResponseEntity<LeaderboardEntry> getPlayerEntry(@PathVariable Long playerId) {
        return leaderboardService.getEntryByPlayerId(playerId)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public LeaderboardEntry addOrUpdateEntry(@RequestBody LeaderboardEntry entry) {
        return leaderboardService.addOrUpdateEntry(entry);
    }

    // Real-time reads served directly from the Redis sorted set (ZSET),
    // O(log N) instead of a Postgres ORDER BY scan.
    @GetMapping("/live/top/{n}")
    public List<Map<String, Object>> getLiveTopN(@PathVariable int n) {
        return liveLeaderboardService.getTopN(n);
    }

    @GetMapping("/live/player/{playerId}")
    public ResponseEntity<Map<String, Object>> getLivePlayerRank(@PathVariable Long playerId) {
        return liveLeaderboardService.getPlayerRank(playerId)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }
}