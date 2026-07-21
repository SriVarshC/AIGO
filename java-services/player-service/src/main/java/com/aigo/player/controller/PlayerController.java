package com.aigo.player.controller;

import com.aigo.player.model.Player;
import com.aigo.player.service.PlayerService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.*;

@RestController
@RequestMapping("/api/players")
public class PlayerController {

    @Autowired
    private PlayerService playerService;

    @GetMapping("/health")
    public Map<String, String> health() {
        Map<String, String> res = new LinkedHashMap<>();
        res.put("status", "UP");
        res.put("service", "player-service");
        res.put("port", "8081");
        return res;
    }

    @GetMapping
    public List<Player> getAllPlayers() {
        return playerService.getAllPlayers();
    }

    @GetMapping("/{id}")
    public ResponseEntity<Player> getPlayerById(@PathVariable Long id) {
        return playerService.getPlayerById(id)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public Player createPlayer(@RequestBody Player player) {
        return playerService.createPlayer(player);
    }

    @PutMapping("/{id}")
    public ResponseEntity<Player> updatePlayer(@PathVariable Long id,
                                               @RequestBody Player player) {
        return playerService.updatePlayer(id, player)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<?> deletePlayer(@PathVariable Long id) {
        if (playerService.deletePlayer(id)) {
            Map<String, String> res = new LinkedHashMap<>();
            res.put("message", "Player deleted successfully");
            return ResponseEntity.ok(res);
        }
        return ResponseEntity.notFound().build();
    }

    @GetMapping("/{id}/stats")
    public ResponseEntity<Map<String, Object>> getPlayerStats(@PathVariable Long id) {
        return playerService.getPlayerStats(id)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @PutMapping("/{id}/elo")
    public ResponseEntity<Player> updateElo(@PathVariable Long id,
                                            @RequestBody Map<String, Integer> body) {
        return playerService.updateElo(id, body.get("eloRating"))
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @GetMapping("/{id}/rank")
    public ResponseEntity<?> getPlayerRank(@PathVariable Long id) {
        Optional<Player> opt = playerService.getPlayerById(id);
        if (opt.isEmpty()) return ResponseEntity.notFound().build();
        Player p = opt.get();
        Map<String, Object> res = new LinkedHashMap<>();
        res.put("id", p.getId());
        res.put("name", p.getName());
        res.put("eloRating", p.getEloRating());
        res.put("rank", playerService.getRank(p.getEloRating()));
        return ResponseEntity.ok(res);
    }

    @GetMapping("/region/{region}")
    public List<Player> getPlayersByRegion(@PathVariable String region) {
        return playerService.getPlayersByRegion(region);
    }
}