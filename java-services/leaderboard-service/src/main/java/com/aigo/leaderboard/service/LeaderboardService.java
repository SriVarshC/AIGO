package com.aigo.leaderboard.service;

import com.aigo.leaderboard.model.LeaderboardEntry;
import com.aigo.leaderboard.repository.LeaderboardRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;
import java.util.*;
import java.util.stream.Collectors;

@Service
public class LeaderboardService {

    @Autowired
    private LeaderboardRepository leaderboardRepository;

    @Autowired
    private LiveLeaderboardService liveLeaderboardService;

    @Cacheable("rankedLeaderboard")
    public List<Map<String, Object>> getRankedLeaderboard() {
        List<LeaderboardEntry> entries = leaderboardRepository.findAllByOrderByEloRatingDesc();
        List<Map<String, Object>> result = new ArrayList<>();
        for (int i = 0; i < entries.size(); i++) {
            LeaderboardEntry e = entries.get(i);
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("position", i + 1);
            row.put("playerId", e.getPlayerId());
            row.put("playerName", e.getPlayerName());
            row.put("region", e.getRegion());
            row.put("eloRating", e.getEloRating());
            row.put("rank", e.getRank());
            row.put("wins", e.getWins());
            row.put("losses", e.getLosses());
            row.put("matchesPlayed", e.getMatchesPlayed());
            row.put("winRate", e.getMatchesPlayed() > 0
                ? String.format("%.1f%%", (double) e.getWins() / e.getMatchesPlayed() * 100)
                : "0%");
            result.add(row);
        }
        return result;
    }

    @Cacheable(value = "topNLeaderboard", key = "#n")
    public List<Map<String, Object>> getTopN(int n) {
        return getRankedLeaderboard().stream().limit(n).collect(Collectors.toList());
    }

    @Cacheable(value = "leaderboardByRegion", key = "#region")
    public List<Map<String, Object>> getLeaderboardByRegion(String region) {
        List<LeaderboardEntry> entries =
            leaderboardRepository.findByRegionOrderByEloRatingDesc(region);
        List<Map<String, Object>> result = new ArrayList<>();
        for (int i = 0; i < entries.size(); i++) {
            LeaderboardEntry e = entries.get(i);
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("position", i + 1);
            row.put("playerId", e.getPlayerId());
            row.put("playerName", e.getPlayerName());
            row.put("region", e.getRegion());
            row.put("eloRating", e.getEloRating());
            row.put("rank", e.getRank());
            row.put("wins", e.getWins());
            row.put("losses", e.getLosses());
            result.add(row);
        }
        return result;
    }

    @CacheEvict(value = {"rankedLeaderboard", "topNLeaderboard", "leaderboardByRegion", "leaderboardEntry"}, allEntries = true)
    public LeaderboardEntry addOrUpdateEntry(LeaderboardEntry entry) {
        LeaderboardEntry saved = leaderboardRepository.findByPlayerId(entry.getPlayerId())
            .map(existing -> {
                existing.setPlayerName(entry.getPlayerName());
                existing.setRegion(entry.getRegion());
                existing.setEloRating(entry.getEloRating());
                existing.setRank(entry.getRank());
                existing.setWins(entry.getWins());
                existing.setLosses(entry.getLosses());
                existing.setMatchesPlayed(entry.getMatchesPlayed());
                return leaderboardRepository.save(existing);
            })
            .orElseGet(() -> leaderboardRepository.save(entry));
        liveLeaderboardService.syncEntry(saved);
        return saved;
    }

    @Cacheable(value = "leaderboardEntry", key = "#playerId")
    public Optional<LeaderboardEntry> getEntryByPlayerId(Long playerId) {
        return leaderboardRepository.findByPlayerId(playerId);
    }
}