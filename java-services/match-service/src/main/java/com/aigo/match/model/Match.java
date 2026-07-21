package com.aigo.match.model;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "matches")
public class Match {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, unique = true)
    private String matchId;

    private Long player1Id;
    private String player1Name;
    private Long player2Id;
    private String player2Name;
    private String winnerName;
    private String arenaName;
    private int rounds;
    private int durationSeconds;
    private LocalDateTime playedAt;

    public Match() {}

    public Match(String matchId, Long player1Id, String player1Name,
                 Long player2Id, String player2Name, String winnerName,
                 String arenaName, int rounds, int durationSeconds,
                 LocalDateTime playedAt) {
        this.matchId = matchId;
        this.player1Id = player1Id;
        this.player1Name = player1Name;
        this.player2Id = player2Id;
        this.player2Name = player2Name;
        this.winnerName = winnerName;
        this.arenaName = arenaName;
        this.rounds = rounds;
        this.durationSeconds = durationSeconds;
        this.playedAt = playedAt;
    }

    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }
    public String getMatchId() { return matchId; }
    public void setMatchId(String matchId) { this.matchId = matchId; }
    public Long getPlayer1Id() { return player1Id; }
    public void setPlayer1Id(Long player1Id) { this.player1Id = player1Id; }
    public String getPlayer1Name() { return player1Name; }
    public void setPlayer1Name(String player1Name) { this.player1Name = player1Name; }
    public Long getPlayer2Id() { return player2Id; }
    public void setPlayer2Id(Long player2Id) { this.player2Id = player2Id; }
    public String getPlayer2Name() { return player2Name; }
    public void setPlayer2Name(String player2Name) { this.player2Name = player2Name; }
    public String getWinnerName() { return winnerName; }
    public void setWinnerName(String winnerName) { this.winnerName = winnerName; }
    public String getArenaName() { return arenaName; }
    public void setArenaName(String arenaName) { this.arenaName = arenaName; }
    public int getRounds() { return rounds; }
    public void setRounds(int rounds) { this.rounds = rounds; }
    public int getDurationSeconds() { return durationSeconds; }
    public void setDurationSeconds(int durationSeconds) { this.durationSeconds = durationSeconds; }
    public LocalDateTime getPlayedAt() { return playedAt; }
    public void setPlayedAt(LocalDateTime playedAt) { this.playedAt = playedAt; }
}