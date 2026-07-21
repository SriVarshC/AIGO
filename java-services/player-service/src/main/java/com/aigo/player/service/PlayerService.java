package com.aigo.player.service;

import com.aigo.player.model.Player;
import com.aigo.player.repository.PlayerRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;
import java.util.*;

@Service
public class PlayerService {

    @Autowired
    private PlayerRepository playerRepository;

    @Cacheable("players")
    public List<Player> getAllPlayers() {
        return playerRepository.findAll();
    }

    @Cacheable(value = "players", key = "#id")
    public Optional<Player> getPlayerById(Long id) {
        return playerRepository.findById(id);
    }

    public Optional<Player> getPlayerByName(String name) {
        return playerRepository.findByName(name);
    }

    @CacheEvict(value = {"players", "playerStats", "playersByRegion"}, allEntries = true)
    public Player createPlayer(Player player) {
        return playerRepository.save(player);
    }

    @CacheEvict(value = {"players", "playerStats", "playersByRegion"}, allEntries = true)
    public Optional<Player> updatePlayer(Long id, Player updated) {
        return playerRepository.findById(id).map(p -> {
            p.setName(updated.getName());
            p.setRegion(updated.getRegion());
            p.setEloRating(updated.getEloRating());
            p.setMatchesPlayed(updated.getMatchesPlayed());
            p.setWins(updated.getWins());
            p.setLosses(updated.getLosses());
            p.setKills(updated.getKills());
            p.setDeaths(updated.getDeaths());
            return playerRepository.save(p);
        });
    }

    @CacheEvict(value = {"players", "playerStats", "playersByRegion"}, allEntries = true)
    public boolean deletePlayer(Long id) {
        if (playerRepository.existsById(id)) {
            playerRepository.deleteById(id);
            return true;
        }
        return false;
    }

    @Cacheable(value = "playerStats", key = "#id")
    public Optional<Map<String, Object>> getPlayerStats(Long id) {
        return playerRepository.findById(id).map(p -> {
            Map<String, Object> stats = new LinkedHashMap<>();
            stats.put("id", p.getId());
            stats.put("name", p.getName());
            stats.put("region", p.getRegion());
            stats.put("eloRating", p.getEloRating());
            stats.put("rank", getRank(p.getEloRating()));
            stats.put("matchesPlayed", p.getMatchesPlayed());
            stats.put("wins", p.getWins());
            stats.put("losses", p.getLosses());
            stats.put("winRate", p.getMatchesPlayed() > 0
                ? String.format("%.1f%%", (double) p.getWins() / p.getMatchesPlayed() * 100)
                : "0%");
            stats.put("kills", p.getKills());
            stats.put("deaths", p.getDeaths());
            stats.put("kda", p.getDeaths() > 0
                ? String.format("%.2f", (double) p.getKills() / p.getDeaths())
                : "INF");
            return stats;
        });
    }

    @CacheEvict(value = {"players", "playerStats", "playersByRegion"}, allEntries = true)
    public Optional<Player> updateElo(Long id, int newElo) {
        return playerRepository.findById(id).map(p -> {
            p.setEloRating(newElo);
            return playerRepository.save(p);
        });
    }

    public String getRank(int elo) {
        if (elo >= 2200) return "GRANDMASTER";
        if (elo >= 2000) return "MASTER";
        if (elo >= 1800) return "DIAMOND";
        if (elo >= 1600) return "PLATINUM";
        if (elo >= 1400) return "GOLD";
        if (elo >= 1200) return "SILVER";
        if (elo >= 1000) return "BRONZE";
        return "IRON";
    }

    @Cacheable(value = "playersByRegion", key = "#region")
    public List<Player> getPlayersByRegion(String region) {
        return playerRepository.findByRegion(region);
    }
}