import React from "react";
import Layout from "@theme/Layout";
import styles from "./index.module.css";

export default function Hello() {
    return (
        <Layout title="NadWin" description="NadWin home page">
            <header className={styles.container}>
                <div className={styles.headerContent}>
                    <div>
                        <h1
                            style={{
                                lineHeight: "0px",
                            }}
                        >
                            NadWin
                        </h1>
                        C++ GUI Library for Windows.
                        <br />
                        <br />
                        Simple, kinda fast and NOT powerful.
                    </div>
                    <img src="img/nadwin-window.png" />
                </div>

                <div className={styles.customShapeDividerBottom1655667759}>
                    <svg data-name="Layer 1" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1200 120" preserveAspectRatio="none">
                        <path
                            d="M0,0V46.29c47.79,22.2,103.59,32.17,158,28,70.36-5.37,136.33-33.31,206.8-37.5C438.64,32.43,512.34,53.67,583,72.05c69.27,18,138.3,24.88,209.4,13.08,36.15-6,69.85-17.84,104.45-29.34C989.49,25,1113-14.29,1200,52.47V0Z"
                            opacity=".25"
                            class="shape-fill"
                        ></path>
                        <path
                            d="M0,0V15.81C13,36.92,27.64,56.86,47.69,72.05,99.41,111.27,165,111,224.58,91.58c31.15-10.15,60.09-26.07,89.67-39.8,40.92-19,84.73-46,130.83-49.67,36.26-2.85,70.9,9.42,98.6,31.56,31.77,25.39,62.32,62,103.63,73,40.44,10.79,81.35-6.69,119.13-24.28s75.16-39,116.92-43.05c59.73-5.85,113.28,22.88,168.9,38.84,30.2,8.66,59,6.17,87.09-7.5,22.43-10.89,48-26.93,60.65-49.24V0Z"
                            opacity=".5"
                            class="shape-fill"
                        ></path>
                        <path
                            d="M0,0V5.63C149.93,59,314.09,71.32,475.83,42.57c43-7.64,84.23-20.12,127.61-26.46,59-8.63,112.48,12.24,165.56,35.4C827.93,77.22,886,95.24,951.2,90c86.53-7,172.46-45.71,248.8-84.81V0Z"
                            class="shape-fill"
                        ></path>
                    </svg>
                </div>
            </header>
            <main className={styles.mainContent}>
                <section
                    className={styles.overviewSection}
                >
                    <p
                        style={{
                            fontSize: "20px",
                            margin: "20px",
                        }}
                    >
                        <span
                            style={{
                                fontSize: "24px",
                            }}
                        >
                            Are you tired of dealing with <b>linker errors</b>, <b>hard api</b> and <b>responsive UI</b> <i>(not this page)</i>?
                        </span>
                        <br />
                        <br />
                        Probably not, but if you do then this library is <i>perfect</i> for you.
                    </p>
                    <div className={styles.customShapeDividerBottom1655670086}>
                        <svg data-name="Layer 1" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1200 120" preserveAspectRatio="none">
                            <path
                                d="M0,0V46.29c47.79,22.2,103.59,32.17,158,28,70.36-5.37,136.33-33.31,206.8-37.5C438.64,32.43,512.34,53.67,583,72.05c69.27,18,138.3,24.88,209.4,13.08,36.15-6,69.85-17.84,104.45-29.34C989.49,25,1113-14.29,1200,52.47V0Z"
                                opacity=".25"
                                class="shape-fill"
                            ></path>
                            <path
                                d="M0,0V15.81C13,36.92,27.64,56.86,47.69,72.05,99.41,111.27,165,111,224.58,91.58c31.15-10.15,60.09-26.07,89.67-39.8,40.92-19,84.73-46,130.83-49.67,36.26-2.85,70.9,9.42,98.6,31.56,31.77,25.39,62.32,62,103.63,73,40.44,10.79,81.35-6.69,119.13-24.28s75.16-39,116.92-43.05c59.73-5.85,113.28,22.88,168.9,38.84,30.2,8.66,59,6.17,87.09-7.5,22.43-10.89,48-26.93,60.65-49.24V0Z"
                                opacity=".5"
                                class="shape-fill"
                            ></path>
                            <path
                                d="M0,0V5.63C149.93,59,314.09,71.32,475.83,42.57c43-7.64,84.23-20.12,127.61-26.46,59-8.63,112.48,12.24,165.56,35.4C827.93,77.22,886,95.24,951.2,90c86.53-7,172.46-45.71,248.8-84.81V0Z"
                                class="shape-fill"
                            ></path>
                        </svg>
                    </div>
                </section>
                <section
                    className={styles.featuresSection}
                >
                    <p
                        style={{
                            display: "flex",
                            justifyContent: "center",
                            alignItems: "center",
                            flexDirection: "row",
                            margin: "0 2vw 20px",
                        }}
                    >
                        <div
                            style={{
                                margin: "0 15px",
                            }}
                        >
                            <h2>Download, include and build.</h2>
                            <br />
                            Thanks to latest technology called <code>Compile from Source</code> you can just include the library and build your GUI.
                        </div>
                        <img
                            style={{
                                width: "50%",
                            }}
                            src="img/header.jpg"
                        />
                    </p>
                    <p
                        style={{
                            display: "flex",
                            justifyContent: "center",
                            alignItems: "center",
                            flexDirection: "row",
                            margin: "20px 2vw",
                        }}
                    >
                        <img
                            style={{
                                width: "50%",
                            }}
                            src="img/truth.png"
                        />
                        <div
                            style={{
                                margin: "0 25px",
                            }}
                        >
                            <h2>No "it works on my machine"</h2>
                            <br />
                            This library is just wrapper to WinAPI which can work on a potato from 90's (as long as it's running Windows XP+ lol).
                            <br />
                            Statistically 1 of 1 (100%) people testing this library was able to get it working on multiple machines.
                        </div>
                    </p>
                </section>
                <a href="https://github.com/Nadwey/NadWin" target="_blank" className={styles.getIt}>
                    <span>Get it on github</span>
                </a>
            </main>
        </Layout>
    );
}
