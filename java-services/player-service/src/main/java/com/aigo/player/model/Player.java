package com.aigo.player.model;

import jakarta.persistence.*;

@Entity
@Table(name = "players")
public class Player {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, unique = true)
    private String name;

    @Column(nullable = false)
    private String region;

    private int eloRating;
    private int matchesPlayed;
    private int wins;
    private int losses;
    private int kills;
    private int deaths;

    public Player() {}

    public Player(String name, String region, int eloRating, int matchesPlayed,
                  int wins, int losses, int kills, int deaths) {
        this.name = name;
        this.region = region;
        this.eloRating = eloRating;
        this.matchesPlayed = matchesPlayed;
        this.wins = wins;
        this.losses = losses;
        this.kills = kills;
        this.deaths = deaths;
    }

    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public String getRegion() { return region; }
    public void setRegion(String region) { this.region = region; }
    public int getEloRating() { return eloRating; }
    public void setEloRating(int eloRating) { this.eloRating = eloRating; }
    public int getMatchesPlayed() { return matchesPlayed; }
    public void setMatchesPlayed(int matchesPlayed) { this.matchesPlayed = matchesPlayed; }
    public int getWins() { return wins; }
    public void setWins(int wins) { this.wins = wins; }
    public int getLosses() { return losses; }
    public void setLosses(int losses) { this.losses = losses; }
    public int getKills() { return kills; }
    public void setKills(int kills) { this.kills = kills; }
    public int getDeaths() { return deaths; }
    public void setDeaths(int deaths) { this.deaths = deaths; }
}