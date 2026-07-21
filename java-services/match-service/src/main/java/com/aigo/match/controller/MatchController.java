package com.aigo.match.controller;

import com.aigo.match.model.Match;
import com.aigo.match.service.MatchService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/matches")
public class MatchController {

    @Autowired
    private MatchService matchService;

    @GetMapping("/health")
    public Map<String, String> health() {
        Map<String, String> res = new LinkedHashMap<>();
        res.put("status", "UP");
        res.put("service", "match-service");
        res.put("port", "8082");
        return res;
    }

    @GetMapping
    public List<Match> getAllMatches() {
        return matchService.getAllMatches();
    }

    @GetMapping("/{id}")
    public ResponseEntity<Match> getMatchById(@PathVariable Long id) {
        return matchService.getMatchById(id)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @GetMapping("/code/{matchId}")
    public ResponseEntity<Match> getMatchByMatchId(@PathVariable String matchId) {
        return matchService.getMatchByMatchId(matchId)
            .map(ResponseEntity::ok)
            .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public Match createMatch(@RequestBody Match match) {
        return matchService.createMatch(match);
    }

    @GetMapping("/player/{playerId}")
    public List<Match> getMatchesByPlayer(@PathVariable Long playerId) {
        return matchService.getMatchesByPlayer(playerId);
    }

    @GetMapping("/arena/{arenaName}")
    public List<Match> getMatchesByArena(@PathVariable String arenaName) {
        return matchService.getMatchesByArena(arenaName);
    }
}