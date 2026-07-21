package com.aigo.leaderboard.model;

import jakarta.persistence.*;

@Entity
@Table(name = "leaderboard")
public class LeaderboardEntry {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, unique = true)
    private Long playerId;

    @Column(nullable = false)
    private String playerName;

    @Column(nullable = false)
    private String region;

    private int eloRating;
    private String rank;
    private int wins;
    private int losses;
    private int matchesPlayed;

    public LeaderboardEntry() {}

    public LeaderboardEntry(Long playerId, String playerName, String region,
                            int eloRating, String rank, int wins, int losses,
                            int matchesPlayed) {
        this.playerId = playerId;
        this.playerName = playerName;
        this.region = region;
        this.eloRating = eloRating;
        this.rank = rank;
        this.wins = wins;
        this.losses = losses;
        this.matchesPlayed = matchesPlayed;
    }

    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }
    public Long getPlayerId() { return playerId; }
    public void setPlayerId(Long playerId) { this.playerId = playerId; }
    public String getPlayerName() { return playerName; }
    public void setPlayerName(String playerName) { this.playerName = playerName; }
    public String getRegion() { return region; }
    public void setRegion(String region) { this.region = region; }
    public int getEloRating() { return eloRating; }
    public void setEloRating(int eloRating) { this.eloRating = eloRating; }
    public String getRank() { return rank; }
    public void setRank(String rank) { this.rank = rank; }
    public int getWins() { return wins; }
    public void setWins(int wins) { this.wins = wins; }
    public int getLosses() { return losses; }
    public void setLosses(int losses) { this.losses = losses; }
    public int getMatchesPlayed() { return matchesPlayed; }
    public void setMatchesPlayed(int matchesPlayed) { this.matchesPlayed = matchesPlayed; }
}