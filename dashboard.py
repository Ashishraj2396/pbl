"""Local educational analysis dashboard."""

import sqlite3
import json
import pandas as pd
import plotly.express as px
from contextlib import closing
from pathlib import Path

import streamlit as st

ROOT = Path(__file__).resolve().parent
DB_PATH = ROOT / "data" / "dashboard.db"

st.set_page_config(
    page_title="Intelligent CCTV Dashboard",
    page_icon="📹",
    layout="wide"
)

st.title("Intelligent CCTV Surveillance System")
st.caption("Educational AI Analysis Dashboard")

if st.sidebar.button("Refresh Reports", use_container_width=True):
    import subprocess
    import sys

    with st.spinner("Importing reports..."):
        result = subprocess.run(
            [sys.executable, str(ROOT / "scripts/import_reports.py")],
            cwd=str(ROOT),
            capture_output=True,
            text=True
        )

    if result.returncode == 0:
        st.sidebar.success("Reports refreshed successfully.")
    else:
        st.sidebar.error("Report import failed.")

if not DB_PATH.is_file():
    st.error("Database not found. Run scripts/import_reports.py first.")
    st.stop()

try:
    with closing(sqlite3.connect(
        DB_PATH.as_uri() + "?mode=ro", uri=True
    )) as conn:

        image_count = conn.execute(
            "SELECT COUNT(*) FROM image_reports"
        ).fetchone()[0]

        video_count = conn.execute(
            "SELECT COUNT(*) FROM video_reports"
        ).fetchone()[0]

        review_count = conn.execute(
            "SELECT COUNT(*) FROM review_records"
        ).fetchone()[0]

except sqlite3.Error as exc:
    st.error(f"Database error: {exc}")
    st.stop()

st.sidebar.title("Navigation")
page = st.sidebar.radio(
    "Select section",
    ["Overview", "Image Reports",
     "Video Reports", "Review History"]
)

st.sidebar.divider()
st.sidebar.caption("Local SQLite Database")

if page == "Overview":
    st.header("System Overview")

    col1, col2, col3 = st.columns(3)

    col1.metric("Image Reports", image_count, border=True)
    col2.metric("Video Reports", video_count, border=True)
    col3.metric("Human Reviews", review_count, border=True)

    st.subheader("Report Statistics")

    summary = pd.DataFrame({
        "Module": ["Image Reports", "Video Reports", "Human Reviews"],
        "Records": [image_count, video_count, review_count]
    })

    fig = px.bar(
        summary,
        x="Module",
        y="Records",
        color_discrete_sequence=["#38BDF8"],
        text_auto=True
    )

    fig.update_layout(height=300)
    fig.update_yaxes(dtick=1)

    st.plotly_chart(fig, use_container_width=True)

    st.success("Database connected successfully.")

    st.subheader("System Modules")

    st.write("✅ Image Object Detection")
    st.write("✅ Recorded Video Analysis")
    st.write("✅ Human Review Management")
    st.write("✅ SQLite Database Integration")

elif page == "Image Reports":
    st.header("Image Detection Reports")

    with closing(sqlite3.connect(
        DB_PATH.as_uri() + "?mode=ro", uri=True
    )) as conn:
        reports = pd.read_sql_query(
            "SELECT * FROM image_reports ORDER BY id DESC",
            conn
        )

    if reports.empty:
        st.info("No image reports available.")
    else:
        selected = st.selectbox(
            "Select detection report",
            reports["source_key"].tolist()
        )

        report = reports[
            reports["source_key"] == selected
        ].iloc[0]

        col1, col2 = st.columns(2)

        col1.metric(
            "Total Detections",
            int(report["total_detections"])
        )

        col2.metric(
            "Confidence Threshold",
            f"{report['confidence_threshold']:.0%}"
        )

        counts = json.loads(report["object_counts"])

        data = pd.DataFrame(
            list(counts.items()),
            columns=["Object", "Count"]
        )

        if not data.empty:
            fig = px.bar(
                data,
                x="Object",
                y="Count",
                color="Object",
                text_auto=True,
                title="Object Detection Summary"
            )

            fig.update_layout(showlegend=False)
            st.plotly_chart(fig, use_container_width=True)

            st.subheader("Detection Details")
            st.dataframe(data, hide_index=True)
        else:
            st.info("No objects detected.")

        st.caption("Counts represent model predictions.")

elif page == "Video Reports":
    st.header("Video Analysis Reports")

    with closing(sqlite3.connect(
        DB_PATH.as_uri() + "?mode=ro", uri=True
    )) as conn:
        reports = pd.read_sql_query(
            "SELECT * FROM video_reports ORDER BY id DESC",
            conn
        )

    if reports.empty:
        st.info("No video reports available.")
    else:
        selected = st.selectbox(
            "Select video report",
            reports["source_key"].tolist()
        )

        report = reports[
            reports["source_key"] == selected
        ].iloc[0]

        c1, c2, c3 = st.columns(3)

        c1.metric("Frames Read", int(report["frames_read"]))
        c2.metric("Sampled Frames", int(report["sampled_frames"]))
        c3.metric("Total Detections", int(report["total_detections"]))

        c4, c5, c6 = st.columns(3)

        c4.metric(
            "Confidence Threshold",
            f"{report['confidence_threshold']:.0%}"
        )
        c5.metric(
            "Sampling Interval",
            int(report["sampling_interval"])
        )
        c6.metric(
            "Processing Time",
            f"{report['processing_seconds']:.3f} s"
        )

        counts = json.loads(report["object_counts"])

        data = pd.DataFrame(
            list(counts.items()),
            columns=["Object", "Count"]
        )

        if not data.empty:
            fig = px.bar(
                data,
                x="Object",
                y="Count",
                color_discrete_sequence=["#38BDF8"],
                text_auto=True,
                title="Aggregate Video Detections"
            )

            st.plotly_chart(fig, use_container_width=True)
            st.dataframe(data, hide_index=True)
        else:
            st.info("No objects detected.")

        st.caption(
            "Counts may include repeated detections "
            "across multiple sampled frames."
        )

elif page == "Review History":
    st.header("Human Review History")

    with closing(sqlite3.connect(
        DB_PATH.as_uri() + "?mode=ro", uri=True
    )) as conn:
        reviews = pd.read_sql_query(
            "SELECT * FROM review_records ORDER BY id DESC",
            conn
        )

    if reviews.empty:
        st.info("No review records available.")
    else:
        accepted = int(
            (reviews["review_decision"] == "ACCEPTED").sum()
        )

        recheck = int(
            (reviews["review_decision"] == "NEEDS_RECHECK").sum()
        )

        c1, c2, c3 = st.columns(3)

        c1.metric("Total Reviews", len(reviews))
        c2.metric("Accepted", accepted)
        c3.metric("Needs Recheck", recheck)

        summary = pd.DataFrame({
            "Status": ["Accepted", "Needs Recheck"],
            "Count": [accepted, recheck]
        })

        fig = px.pie(
            summary,
            names="Status",
            values="Count",
            hole=0.45,
            title="Review Decision Summary",
            color_discrete_sequence=["#22C55E", "#F59E0B"]
        )

        st.plotly_chart(fig, use_container_width=True)

        st.subheader("Review Records")

        st.dataframe(
            reviews[[
                "timestamp",
                "confidence_threshold",
                "total_detections",
                "review_decision"
            ]],
            hide_index=True
        )

        st.caption(
            "Human review decisions do not establish "
            "identity, authorization or model accuracy."
        )

st.divider()
st.caption(
    "Academic PBL Project | Approved Sample Analysis | "
    "Aggregate Model Predictions"
)
